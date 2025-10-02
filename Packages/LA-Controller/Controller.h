#pragma once
#include <thread>
#include <atomic>
#include <chrono>

//HID INCLUDES
#include "HID/hidapi.h"

//LA_CONTROLLER INCLUDES
#include "Export.h"
#include "Vector.h"

#include "Identification.h"
#include "ControllerType.h"
#include "Button.h"
#include "Joystick.h"
#include "Trigger.h"
#include "Dpad.h"

#define SONY_VENDOR_ID 0x054C
#define DUALSENSE_PRODUCT_ID 0x0CE6

namespace la {
	int LA_CONTROLLER_API Init();

	namespace Freq {
		enum Frequency {
			high,
			low
		};
	}


	class LA_CONTROLLER_API Event {
	public:
		[[nodiscard]] bool isConnected();
		[[nodiscard]] bool isDisconnected();
	};

	// Déclaration anticipée pour Controller

	class LA_CONTROLLER_API Properties {
		friend class Controller;

	private:
		bool var_hasAudioOutput;
		bool var_hasMicrophone;
		bool var_hasGyroscope;
		bool var_hasTouchPad;
		bool var_hasLED;
		unsigned int paddlesCount;

	public:
		//audio
		[[nodiscard]] bool hasAudioOutput();
		[[nodiscard]] bool hasMicrophone();

		//misc
		[[nodiscard]] bool hasGyroscope();
		[[nodiscard]] bool hasTouchPad();

		//led
		[[nodiscard]] bool hasLED();
		void setLedColor();

		//paddles
		[[nodiscard]] bool hasPaddles();
		unsigned int getPaddlesCount();
	};

	class LA_CONTROLLER_API Controller {
	private:
		//controller identification
		Identification identification;

		//properties
		Properties properties;

		//event
		Event event;

		bool connected;

		// Variables pour la vibration
		std::atomic<bool> isVibrating;
		std::thread vibrationThread;

	public:
		//Constructor
		Controller();
		~Controller();

		// Méthodes de connexion et identification
		bool connectController(unsigned int vendorId = 0, unsigned int productId = 0);
		bool autoDetectAndConnect();
		void disconnectController();

		// Getters pour identification
		std::string getControllerName() const;
		unsigned int getVendorID() const;
		unsigned int getProductID() const;
		const wchar_t *getSerialNumber() const;

		//buttons
		Button button;

		//joysticks
		Joystick rightJoystick;
		Joystick leftJoystick;
		DPAD dpad;

		//triggers
		Trigger rightTrigger;
		Trigger leftTrigger;

		//functions - ENLEVÉ inline
		Event &getEvent();
		Identification &getIdentification();
		Properties &getProperties();

		[[nodiscard]] bool isConnected();
		[[nodiscard]] bool hasAudioOutput();

		// Méthodes pour vérifier le type de contrôleur
		[[nodiscard]] bool isDualSense() const;
		[[nodiscard]] bool isXboxController() const;

		void vibrate(float duration);
		void vibrateWithIntensity(float duration, float leftIntensity = 1.0f, float rightIntensity = 1.0f);
		void stopVibration();
		bool getIsVibrating() const;

		// Méthode pour mettre à jour tous les inputs
		void updateInputs();

	private:
		void sendVibrationData(float leftIntensity, float rightIntensity);
		void vibrationWorker(float duration, float leftIntensity, float rightIntensity);
	};
}