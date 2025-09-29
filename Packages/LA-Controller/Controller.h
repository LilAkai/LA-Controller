#pragma once
#include <thread>
#include <atomic>
#include <chrono>

//HID INCLUDES
#include "HID/hidapi.h"
//#include "HID/hidapi_winapi.h"

//LA_CONTROLLER INCLUDES
#include "Vector.h"
#include "Identification.h"
#include "Export.h"

#define SONY_VENDOR_ID 0x054C
#define DUALSENSE_PRODUCT_ID 0x0CE6

namespace la {
	inline int Init() {
		return hid_init();
	}

	namespace Freq {
		enum Frequency {
			high,
			low
		};
	}

	namespace ControllerType {
		enum Xbox {
			A = 0,
			B,
			X,
			Y,

			Start = 7,
			Select = 6,

			LB = 4,
			RB = 5,

			LStick_Xbox = 8,
			RStick_Xbox = 9
		};

		enum Dualsense {
			Cross = 0,
			Circle,
			Square,
			Triangle,

			Options = 7,
			Share = 6,

			L1 = 4,
			R1 = 5,

			LStick_DS = 8,
			RStick_DS = 9
		};
	}

	class LA_CONTROLLER_API Button {
	private:
		static constexpr unsigned int maxButtons {32};
	public:
		[[nodiscard]] bool isPressed(la::ControllerType::Dualsense button);
		[[nodiscard]] bool isPressed(la::ControllerType::Xbox button);
	};

	class LA_CONTROLLER_API Joystick {
	private:
		la::Vector2f axis;
		static constexpr unsigned int axisCount {8};
	public:
		// Constructeur par défaut
		Joystick();
		la::Vector2f getAxis();
	};

	class LA_CONTROLLER_API Trigger {
	private:
		float value;
	public:
		// Constructeur par défaut
		Trigger();
		float getValue();
	};

	class LA_CONTROLLER_API Event {
	public:
		[[nodiscard]] bool isConnected();
		[[nodiscard]] bool isDisconnected();
	};

	// Déclaration anticipée pour Controller
	class Controller;

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

		// Getters pour identification - ENLEVÉ inline
		std::string getControllerName() const;
		unsigned int getVendorID() const;
		unsigned int getProductID() const;
		const wchar_t *getSerialNumber() const;

		//buttons
		Button button;

		//joysticks
		Joystick rightJoystick;
		Joystick leftJoystick;

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

	private:
		void sendVibrationData(float leftIntensity, float rightIntensity);
		void vibrationWorker(float duration, float leftIntensity, float rightIntensity);
	};
}