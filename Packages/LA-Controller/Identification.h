#pragma once
#include "Export.h"
#include "HID/hidapi.h"

#include <string>
#include <vector>

namespace la {
	class LA_CONTROLLER_API Identification {
	private:
		hid_device *device;
		std::string name;
		unsigned int vendorID;
		unsigned int productID;
		wchar_t serialNumber[256];
		bool initialized;

	public:
		// Constructeur
		Identification();
		~Identification();

		// Méthodes pour initialiser et connecter
		bool initialize();
		bool connectToController(unsigned int vendorId = 0, unsigned int productId = 0);
		void disconnect();

		// Getters publics
		std::string getName() const;
		unsigned int getVendorID() const;
		unsigned int getProductID() const;
		const wchar_t *getSerialNumber() const;
		bool isInitialized() const;

		// Méthodes statiques pour lister les contrôleurs
		static std::vector<hid_device_info *> listControllers();
		static void freeControllersList(std::vector<hid_device_info *> &list);

		// Méthodes pour vérifier le type de contrôleur
		bool isDualSense() const;
		bool isXboxController() const;

		// Getter pour le handle HID (pour les opérations avancées)
		hid_device *getDevice() const;
	};
}