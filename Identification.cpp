#include "pch.h"
#include "Identification.h"

#include <iostream>

la::Identification::Identification()
    : device(nullptr), name("No Controller"), vendorID(0), productID(0),
    serialNumber(L""), initialized(false) {}

la::Identification::~Identification() {
    disconnect();
}

bool la::Identification::initialize() {
    if (hid_init()!=0) {
        std::cerr<<"Erreur lors de l'initialisation de HID API"<<std::endl;
        return false;
    }
    initialized = true;
    return true;
}

bool la::Identification::connectToController(unsigned int vendorId, unsigned int productId) {
    if (!initialized&&!initialize()) {
        return false;
    }

    // Si pas de paramètres spécifiés, chercher automatiquement
    if (vendorId==0&&productId==0) {
        auto controllers = listControllers();
        if (!controllers.empty()) {
            // Prendre le premier contrôleur trouvé
            vendorId = controllers[0]->vendor_id;
            productId = controllers[0]->product_id;

            // Copier les informations
            if (controllers[0]->product_string) {
                std::wstring ws(controllers[0]->product_string);
                name = std::string(ws.begin(), ws.end());
            }

            // Copier le serialNumber en wchar_t*
            if (controllers[0]->serial_number) {
                wcscpy_s(serialNumber, 256, controllers[0]->serial_number);
            } else {
                serialNumber[0] = L'\0'; // Chaîne vide si pas de serial
            }
        }
        freeControllersList(controllers);
    }

    // Tenter de se connecter au contrôleur
    device = hid_open(vendorId, productId, nullptr);
    if (!device) {
        std::cerr<<"Impossible d'ouvrir le contrôleur (VID: 0x"
            <<std::hex<<vendorId<<", PID: 0x"<<productId<<")"<<std::endl;
        return false;
    }

    // Sauvegarder les IDs
    this->vendorID = vendorId;
    this->productID = productId;

    // Si le nom n'a pas été récupéré, essayer de le lire depuis le device
    if (name=="No Controller") {
        wchar_t product_string[256];
        if (hid_get_product_string(device, product_string, 256)==0) {
            std::wstring ws(product_string);
            name = std::string(ws.begin(), ws.end());
        }
    }

    // Récupérer le numéro de série si pas encore fait
    if (serialNumber[0]==L'\0') { // Vérifier si la chaîne wchar_t est vide
        if (hid_get_serial_number_string(device, serialNumber, 256)!=0) {
            // En cas d'erreur, mettre une chaîne vide
            serialNumber[0] = L'\0';
        }
    }

    std::cout<<"Contrôleur connecté: "<<name<<std::endl;
    return true;
}

void la::Identification::disconnect() {
    if (device) {
        hid_close(device);
        device = nullptr;
    }
    if (initialized) {
        hid_exit();
        initialized = false;
    }
}

std::string la::Identification::getName() const {
    return name;
}

unsigned int la::Identification::getVendorID() const {
    return vendorID;
}

unsigned int la::Identification::getProductID() const {
    return productID;
}

const wchar_t *la::Identification::getSerialNumber() const {
    return serialNumber;
}

bool la::Identification::isInitialized() const {
    return initialized&&device!=nullptr;
}

std::vector<hid_device_info *> la::Identification::listControllers() {
    std::vector<hid_device_info *> controllers;

    // Liste des VID/PID connus pour les contrôleurs gaming
    struct ControllerID {
        unsigned short vendor_id;
        unsigned short product_id;
        const char *name;
    };

    // Contrôleurs supportés (vous pouvez étendre cette liste)
    const ControllerID knownControllers[] = {
        {0x054c, 0x0ce6, "DualSense"},           // PS5 DualSense
        {0x054c, 0x0df2, "DualSense Edge"},      // PS5 DualSense Edge
                                                 
        {0x054c, 0x09cc, "DualShock 4"},         // PS4 DualShock 4
        {0x045e, 0x02ea, "Xbox One"},            // Xbox One Controller
        {0x045e, 0x0b12, "Xbox Series"},         // Xbox Series X|S Controller
        {0x045e, 0x0b00, "Xbox Elite Series 2"}, // Xbox Elite Series Controller
    };

    hid_device_info *devices = hid_enumerate(0x0, 0x0);
    hid_device_info *current = devices;

    while (current) {
        // Vérifier si c'est un contrôleur connu
        for (const auto &controller:knownControllers) {
            if (current->vendor_id==controller.vendor_id&&
                current->product_id==controller.product_id) {
                controllers.push_back(current);
                break;
            }
        }
        current = current->next;
    }

    return controllers;
}

void la::Identification::freeControllersList(std::vector<hid_device_info *> &list) {
    if (!list.empty()) {
        // Libérer la liste entière (HID API libère toute la chaîne)
        hid_free_enumeration(list[0]);
        list.clear();
    }
}

bool la::Identification::isDualSense() const {
    return vendorID==0x054C && productID==0x0CE6;
}

bool la::Identification::isXboxController() const {
    return vendorID==0x045E && (productID==0x02EA||productID==0x0B12);
}

hid_device *la::Identification::getDevice() const {
    return device;
}