#include "pch.h"
#include "LA-Controller/Controller.h"

#include <iostream>
#include <thread>
#include <chrono>
#include <algorithm>

int la::Init() {
    return hid_init();
}

la::Controller::Controller(): button(*this), connected(false), isVibrating(false) {
	properties.var_hasAudioOutput = false;
	properties.var_hasMicrophone = false;
	properties.var_hasGyroscope = false;
	properties.var_hasTouchPad = false;
	properties.var_hasLED = false;
	properties.paddlesCount = 0;
}

la::Controller::~Controller() {
    stopVibration();
    disconnectController();
}

std::string la::Controller::getControllerName() const {
    return identification.getName();
}

unsigned int la::Controller::getVendorID() const {
    return identification.getVendorID();
}

unsigned int la::Controller::getProductID() const {
    return identification.getProductID();
}

const wchar_t *la::Controller::getSerialNumber() const {
    return identification.getSerialNumber();
}

la::Event &la::Controller::getEvent() {
    return this->event;
}

la::Identification &la::Controller::getIdentification() {
    return this->identification;
}

la::Properties &la::Controller::getProperties() {
    return this->properties;
}

bool la::Controller::isConnected() {
    return connected&&identification.isInitialized();
}

bool la::Controller::hasAudioOutput() {
    return properties.hasAudioOutput();
}

bool la::Controller::isDualSense() const {
    return identification.isDualSense();
}

bool la::Controller::isXboxController() const {
    return identification.isXboxController();
}

bool la::Controller::connectController(unsigned int vendorId, unsigned int productId) {
    if (connected) {
        disconnectController();
    }

    bool success = identification.connectToController(vendorId, productId);
    if (success) {
        connected = true;
        std::cout<<"Contrôleur connecté: "<<getControllerName()
            <<" (VID: 0x"<<std::hex<<getVendorID()
            <<", PID: 0x"<<getProductID()<<")"<<std::endl;

        // Configurer les propriétés selon le type de contrôleur
        if (isDualSense()) {
            properties.var_hasAudioOutput = true;
            properties.var_hasMicrophone = true;
            properties.var_hasGyroscope = true;
            properties.var_hasTouchPad = true;
            properties.var_hasLED = true;
            properties.paddlesCount = 0;
        } else if (isXboxController()) {
            properties.var_hasAudioOutput = true;
            properties.var_hasMicrophone = false;
            properties.var_hasGyroscope = false;
            properties.var_hasTouchPad = false;
            properties.var_hasLED = false;
            properties.paddlesCount = 0;
        }
    }

    return success;
}

bool la::Controller::autoDetectAndConnect() {
    return connectController(0, 0);
}

void la::Controller::disconnectController() {
    if (connected) {
        identification.disconnect();
        connected = false;
        std::cout<<"Contrôleur déconnecté"<<std::endl;
    }
}


// Implémentations des méthodes Properties
bool la::Properties::hasAudioOutput() {
    return this->var_hasAudioOutput;
}

bool la::Properties::hasMicrophone() {
    return this->var_hasMicrophone;
}

bool la::Properties::hasGyroscope() {
    return this->var_hasGyroscope;
}

bool la::Properties::hasTouchPad() {
    return this->var_hasTouchPad;
}

bool la::Properties::hasLED() {
    return this->var_hasLED;
}

void la::Properties::setLedColor() {
    // TODO: Implémenter selon le type de contrôleur
}

bool la::Properties::hasPaddles() {
    return this->paddlesCount>0;
}

unsigned int la::Properties::getPaddlesCount() {
    return this->paddlesCount;
}



void la::Controller::vibrate(float duration) {
    vibrateWithIntensity(duration, 1.0f, 1.0f);
}

void la::Controller::vibrateWithIntensity(float duration, float leftIntensity, float rightIntensity) {
    if (!isConnected()) {
        std::cerr<<"Aucun contrôleur connecté pour la vibration"<<std::endl;
        return;
    }

    if (duration<=0.0f) {
        std::cerr<<"Durée de vibration invalide: "<<duration<<std::endl;
        return;
    }

    stopVibration();

    leftIntensity = std::clamp(leftIntensity, 0.0f, 1.0f);
    rightIntensity = std::clamp(rightIntensity, 0.0f, 1.0f);

    isVibrating = true;
    vibrationThread = std::thread(&Controller::vibrationWorker, this, duration, leftIntensity, rightIntensity);
}

void la::Controller::stopVibration() {
    if (isVibrating) {
        isVibrating = false;
        if (vibrationThread.joinable()) {
            vibrationThread.join();
        }
        sendVibrationData(0.0f, 0.0f);
    }
}

bool la::Controller::getIsVibrating() const {
    return isVibrating;
}

void la::Controller::vibrationWorker(float duration, float leftIntensity, float rightIntensity) {
    sendVibrationData(leftIntensity, rightIntensity);

    auto startTime = std::chrono::steady_clock::now();
    auto durationMs = std::chrono::milliseconds(static_cast<int>(duration*1000));

    while (isVibrating) {
        auto currentTime = std::chrono::steady_clock::now();
        if (currentTime-startTime>=durationMs) {
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    sendVibrationData(0.0f, 0.0f);
    isVibrating = false;

    std::cout<<"Vibration terminée après "<<duration<<"s"<<std::endl;
}

void la::Controller::sendVibrationData(float leftIntensity, float rightIntensity) {
    if (!isConnected()) {
        return;
    }

    hid_device *device = identification.getDevice();
    if (!device) {
        return;
    }

    if (isDualSense()) {
        unsigned char vibrationData[48] = {0};
        vibrationData[0] = 0x02;
        vibrationData[1] = 0xFF;
        vibrationData[2] = 0xF7;
        vibrationData[3] = static_cast<unsigned char>(rightIntensity*255);
        vibrationData[4] = static_cast<unsigned char>(leftIntensity*255);

        int result = hid_write(device, vibrationData, sizeof(vibrationData));
        if (result<0) {
            std::cerr<<"Erreur lors de l'envoi de la vibration DualSense"<<std::endl;
        }

    } else if (isXboxController()) {
        unsigned char vibrationData[8] = {0};
        vibrationData[0] = 0x03;
        vibrationData[1] = 0x08;
        vibrationData[2] = static_cast<unsigned char>(leftIntensity*255);
        vibrationData[3] = static_cast<unsigned char>(rightIntensity*255);
        vibrationData[4] = 255;
        vibrationData[5] = 255;
        vibrationData[6] = 255;
        vibrationData[7] = 255;

        int result = hid_write(device, vibrationData, sizeof(vibrationData));
        if (result<0) {
            std::cerr<<"Erreur lors de l'envoi de la vibration Xbox"<<std::endl;
        }
    }
}

// Méthode pour mettre à jour tous les inputs (à appeler dans votre boucle de jeu)
void la::Controller::updateInputs() {
    if (isConnected()) {
        this->button.updateButtonStates(); //TODO : mettre a jour les joysticks et les triggers
    }
}