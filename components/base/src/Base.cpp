/**********************************************************************************************************************/
/* INCLUSIONS                                                                                                         */
/**********************************************************************************************************************/

#include "Base.h"
#include <utility>

String Base::wifi_connect(WiFiMulti * wiFiMulti) {
    String ip = "";

    for(uint8_t t = 4; t > 0; t--) {
        delay(DELAY);
    }

    // disable AP
    if(WiFi.getMode() & WIFI_AP) {
        WiFi.softAPdisconnect(true);
    }

    wiFiMulti->addAP(WIFI_SSID, WIFI_PASS);

    while(wiFiMulti->run() != WL_CONNECTED) {
        delay(DELAY/10);

        return "";
    }

    ip = WiFi.localIP().toString();
    M5.Lcd.printf("[SETUP] WiFi Connected %s\n", ip.c_str());

    return ip.c_str();
}

void Base::initialNFCReader(MFRC522 * rfid_param) {
    //SPI.begin(); // init SPI bus
    //rfid_param->PCD_Init(); // init MFRC522
}

String Base::readerCard(MFRC522 * mfrc522) {
//    string tag_read = "832338BC"; // Default value to delete soon
    String rfid_read = "";
    if (!mfrc522->PICC_IsNewCardPresent() || !mfrc522->PICC_ReadCardSerial()) {
        delay(200);

        return rfid_read;
    }

    if (mfrc522->uid.size > 0) {
        for (byte i = 0; i < mfrc522->uid.size; i++) {
            rfid_read += String(mfrc522->uid.uidByte[i], HEX);
        }
    }

    Serial.println(rfid_read);

    return rfid_read;
}

int Base::readProduct(const String& rfid_read_product_ref) {
    int return_value = BASE_EXIT_CODE::SUCCESS;

    if (!rfid_read_product_ref.isEmpty()) {
        String uri = String("/products/ref/")  + rfid_read_product_ref;
        String api_response =  Request::requestGet(uri.c_str(), this->getToken().c_str());

        if (api_response.length()) {
            // TODO : to optimize the Dynamic allocation size
            /*const int size_to_alloc = api_response.length() + 1;
            DynamicJsonDocument doc(8192);
            DynamicJsonDocument doc1(1024);*/

            const char* response_to_be_deserialized = api_response.c_str();

            JSONVar deserialized_response = JSON.parse(response_to_be_deserialized);

            Serial.println(deserialized_response);

            //deserializeJson(doc, api_response);
            //const char* id = deserialized_response["id"];
            //this->setCurrentProductId((string) deserialized_response["id"]);

            this->currentProductId = (string) deserialized_response["id"];

            //const char* fk_default_warehouse = deserialized_response["fk_default_warehouse"];
            //this->setWareHouse((string)deserialized_response["fk_default_warehouse"]);

            string currentWhToBeConverted = (string) deserialized_response["fk_default_warehouse"];
            Serial.printf("wh : %s\n", currentWhToBeConverted.c_str());

            if (strcmp(currentWhToBeConverted.c_str(), "1") == 0) {
                this->currentWarehouse = 1;
            } else if (strcmp(currentWhToBeConverted.c_str(), "2") == 0) {
                this->currentWarehouse = 2;
            } else if (strcmp(currentWhToBeConverted.c_str(), "3") == 0) {
                this->currentWarehouse = 3;
            } else {
                this->currentWarehouse = -1;
            }

            switch (this->currentWarehouse) {
                case 1:
                    this->currentProductExit = LEFT_EXIT;
                    break;
                case 2:
                    this->currentProductExit = MIDDLE_EXIT;
                    break;
                case 3:
                    this->currentProductExit = RIGHT_EXIT;
                    break;
                default:
                    this->currentProductExit = -1;
                    break;
            }

            //const char* array_options = doc["array_options"];
            //deserializeJson(doc1, array_options);
            int movementId = (int) deserialized_response["array_options"]["options_movement_id"];

            Serial.printf("******************************\n");
            Serial.printf("product id : %s\n", this->currentProductId.c_str());
            Serial.printf("product warehouse : %d\n", this->currentWarehouse);
            Serial.printf("product exit : %d\n", this->currentProductExit);
            Serial.printf("movement id : %d\n", movementId);
            Serial.printf("******************************\n");

            if (movementId != 0) {
                return_value = BASE_EXIT_CODE::ALREADY_CHECKED;
            }
        } else {
            Base::printOnLCD(reinterpret_cast<basic_string<char> &&>("No product found : " + api_response));
            return_value = BASE_EXIT_CODE::NO_PRODUCT_FOUND;
        }
    }

    return return_value;
}

int Base::createMovement() {

    int currentMovementId = 0;
    // NOTE : Status 400 : Bad Request
    const string uri = "/stockmovements";
    const String data = String(R"({"product_id": )")  + this->getCurrentProductId().c_str()
                        + String(R"(,"warehouse_id": )") + this->getWareHouse()//.c_str()
                        + String(R"(,"qty": ")") + String(1)
                        + String("\"}");
    String response = Request::requestPost(uri.c_str(), data, this->token.c_str());

    if (response.length()) {
        currentMovementId = response.toInt();
    } else {
        Base::printOnLCD(reinterpret_cast<basic_string<char> &&>("No movement created : " + response));
    }

    return currentMovementId;
}

void Base::commitTransaction(int movement_id) {
    // Prepare url and data
    const string separator = "/products/";
    string uri = separator + this->currentProductId;
    const String data = String(R"({"array_options": {"options_movement_id": )") +
                        movement_id + String("}}");

    String response = Request::requestPut(uri.c_str(), data, this->token.c_str());

    unsigned int response_size = response.length() + 1;
    /*DynamicJsonDocument doc(response_size);
    deserializeJson(doc, response);*/

    const char* response_to_be_deserialized = response.c_str();

    JSONVar deserialized_response = JSON.parse(response_to_be_deserialized);

    const String updated_product_id = deserialized_response["id"];

    // Test if the put method worked
    if (updated_product_id.compareTo(this->currentProductId.c_str()) != 0)   {
        Base::printOnLCD("ERROR: Transaction not committed");
        return;
    }
    this->currentProductId = "";
}

void Base::login(String data) {
    const String response = Request::requestPost("/login", data);
    /*DynamicJsonDocument doc(sizeof response);
    DynamicJsonDocument doc1(2048);
    deserializeJson(doc, response);
    const char* world = doc["success"];
    deserializeJson(doc1, world);
    const char* token_ = doc1["token"];*/

    const char* response_to_be_deserialized = response.c_str();

    JSONVar deserialized_response = JSON.parse(response_to_be_deserialized);

    this->token = (string)deserialized_response["success"]["token"];

    Serial.printf("My final token : %s\n", this->token.c_str());
}

String Base::checkNFC(const string nfc_code) {

    const string separator = "/products/ref/";
    string uri = separator + nfc_code;

    return Request::requestGet(uri.c_str(), this->getToken().c_str());
}

string Base::getToken() {
    return this->token;
}

void Base::printOnLCD(const string& message) {
    //M5.Lcd.clearDisplay();
    //M5.Lcd.setCursor(0, 10);
    M5.Lcd.printf("%s\n", message.c_str());
}

// Getters and Setters
string Base::getCurrentProductId() {
    return this->currentProductId;
}

void Base::setCurrentProductId(string currentProductId) {
    this->currentProductId = currentProductId;
}

int Base::getCurrentProductExit() {
    return this->currentProductExit;
}

void Base::setCurrentProductExit(int exit_value) {
    this->currentProductExit = exit_value;
}

int Base::getWareHouse() {
    return this->currentWarehouse;
}

void Base::setWareHouse(int warehouse) {
    this->currentWarehouse = warehouse;
}