/**********************************************************************************************************************/
/* INCLUSIONS                                                                                                         */
/**********************************************************************************************************************/

#include "../components/base/src/Base.h"

/**********************************************************************************************************************/
/* CLASS INSTANCES                                                                                                    */
/**********************************************************************************************************************/

// Wi-Fi connection
WiFiMulti wiFiMulti;

// Servo motor
ServoMotor servoMotor;

// Base which contain everything about NFC
Base base;

// NFC READER
MFRC522 mfrc522(0x28);  // Create MFRC522 instance

// NVS
Preferences preferences;

/**********************************************************************************************************************/
/* PRIVATE CONSTANTS AND VARIABLES                                                                                    */
/**********************************************************************************************************************/

// API user credentials
const String api_user_data = "{\"login\":\"" API_USER "\", \"password\":\"" API_PASSWORD "\"}";
String wifi_connection_ip_adderss = "";


/**********************************************************************************************************************/
/* PRIVATE FUNCTION PROTOTYPE                                                                                         */
/**********************************************************************************************************************/

// Setup public function methods prototype
static void board_main_configuration();
static void rfid_configuration();
static void header(const char *string, uint16_t color);

// Loop public function methods prototype


/**********************************************************************************************************************/
/* PRIVATE FUNCTION CODE                                                                                              */
/**********************************************************************************************************************/

static void board_main_configuration()
{
    M5.begin(true, false, true, true);
    M5.Power.begin(); // Init power.

    M5.Lcd.setTextSize(2);
    M5.Lcd.setCursor(70, 0);
    M5.Lcd.println("T-IOT-901-STG");
}

static void rfid_configuration()
{
    M5.Lcd.println("Scan a RFID card");
    Wire.begin();  // Wire init, adding the I2C bus.
    mfrc522.PCD_Init();  // Init MFRC522.

}

static void header(const char *string, uint16_t color)
{
    M5.Lcd.fillScreen(color);
    M5.Lcd.setTextSize(1);
    M5.Lcd.setTextColor(TFT_WHITE, TFT_BLACK);
    M5.Lcd.fillRect(0, 0, 320, 30, TFT_BLACK);
    M5.Lcd.setTextDatum(TC_DATUM);
    M5.Lcd.drawString(string, 160, 3, 4);
}

/**********************************************************************************************************************/
/* PUBLIC FUNCTION CODE                                                                                               */
/**********************************************************************************************************************/

void setup() {
    // Board main configuration
    board_main_configuration();

    // Connect Wifi  (BLOCKING)
    wifi_connection_ip_adderss = Base::wifi_connect(&wiFiMulti);
    if (wifi_connection_ip_adderss != "") {
        // Login user in the order to get API token
        while (base.getToken().length() == 0) {
            base.login(api_user_data);
        }

        // RFID configuration
        rfid_configuration();

        // Step motor configuration
        StepMotor::step_motor_configuration();

        // Servo motor configuration
        ServoMotor::servo_motor_configuration();

    } else {
        M5.Lcd.println("There was an connection issue!");
    }
}

void loop() {

    int axe_xyz = 1800;
    axe_xyz += 1800;

    if (M5.BtnA.wasPressed()) {
        StepMotor::step_motor_run(axe_xyz, 600);
    } else if (M5.BtnB.wasPressed()) {
       StepMotor::step_motor_inlock();
    } else if (M5.BtnC.wasPressed()) {
        StepMotor::step_motor_unlock();
    }

    M5.update();

    // to review
    M5.Lcd.setCursor(40, 47);

    // Reading the RFID reference and save it into the variable
    String rfid_read_product_ref = Base::readerCard(&mfrc522);
    if (rfid_read_product_ref.isEmpty()) {
        return;
    }

    // Reading the product from the warehouse
    int scan_result = base.readProduct(rfid_read_product_ref);

    // If the product is scaned for the first time : save currentid and whare hous info in nvs and switch nvs to find exit
    // or already checked
    if (base.getToken().length() > 0 && (scan_result  == BASE_EXIT_CODE::SUCCESS || scan_result  == BASE_EXIT_CODE::ALREADY_CHECKED)) {
        // read exit from nvs and move to the appropriate angle
        preferences.begin("product-info", false);

        preferences.putInt("pwh", base.getWareHouse());
        preferences.putInt("pex", base.getCurrentProductExit());
        preferences.putString("pid", base.getCurrentProductId().c_str());

        Serial.printf("Redirection from main-nvs: %d\n", preferences.getInt("pex", -1));

        if (preferences.getInt("pex", -1) != -1) {
            ServoMotor::servo_motor_move_arm(preferences.getInt("pex", -1));
        } else {
            Serial.println("redirection error");
        }


        preferences.end();

        //delay(DELAY);
        //int movement_id = base.createMovement();

        // Must Commit the Transaction once the product is out of conveyor
        //base.commitTransaction(movement_id);
    } else {
        // stop the covoyeur
        // turn in opposite side to roll back the package
    }
}




/**********************************************************************************************************************/
/* END OF FILE                                                                                                        */
/**********************************************************************************************************************/
