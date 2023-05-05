/**********************************************************************************************************************/
/* MAIN DEFINITIONS                                                                                                   */
/**********************************************************************************************************************/

#ifndef BASE_H
#define BASE_H

/**********************************************************************************************************************/
/* INCLUSIONS                                                                                                         */
/**********************************************************************************************************************/

#include "main.h"

/**********************************************************************************************************************/
/* PRIVATE ENUM AND VARIABLE                                                                                          */
/**********************************************************************************************************************/

enum BASE_EXIT_CODE {
    SUCCESS,
    ERROR,
    ALREADY_CHECKED,
    NO_PRODUCT_FOUND,
};

/**********************************************************************************************************************/
/* PUBLIC CLASS                                                                                                       */
/**********************************************************************************************************************/
class Base {

private:

/**********************************************************************************************************************/
/* PRIVATE ATTRIBUTES                                                                                                 */
/**********************************************************************************************************************/

    string token;
    string currentProductId;
    int currentWarehouse = -1;
    int currentProductExit = -1;

/**********************************************************************************************************************/
/* PRIVATE METHODS PROTOTYPES                                                                                         */
/**********************************************************************************************************************/

public:

/**********************************************************************************************************************/
/* PUBLIC ATTRIBUTES                                                                                                  */
/**********************************************************************************************************************/


/**********************************************************************************************************************/
/* PUBLIC METHODS PROTOTYPES                                                                                          */
/**********************************************************************************************************************/

    /**
     * Connect to Wifi
     * @param WiFiMulti
     */
    static String wifi_connect(WiFiMulti * WiFiMulti);

    /**
     * Init Sensors
     * @param rfid_param
     */
    static void initialNFCReader(MFRC522 * rfid_param);


    /**
     * @brief Get the Sensor Tag. This tag is used to identify the sensor in the database
     * @param rfid_param
     * @return return the product id or null if not found
     */
    static String readerCard(MFRC522 * rfid_param);

    /**
      this method return the exit of the product
      -1 : not product read
       this return value is between 1 and 3
    */
//   void readProduct(MFRC522 *rfid_param, ServoMotor *servoMotor_param);
    int readProduct(const String& rfid_read_product_ref);

    void setCurrentProductExit(int exit_value);
    int getCurrentProductExit();

    /**
     *
     */
    String checkNFC(const string nfc_code);

    /**
     * Commit Transaction is to finish the transaction making API call
     * with the status value to 1
     */
    void commitTransaction(int movement_id);

    /**
     * @brief This method is used to login and get the token
     * @param data
     */
    void login(String data);

    string getToken();

    static void printOnLCD(const string& message);

    /**
     * @brief This method will user 2 variables to store the current product id and the Warehouse id
     * @return the currentMovementId
     */
    int createMovement();

    /**
     * @brief This method is used to update the movement id
     * @param movement_id
     */
    void setWareHouse(int warehouse);

    /**
     * @brief This method is used to get the current Movement Id
     * @return
     */
    int getWareHouse();

    string getCurrentProductId();

    void setCurrentProductId(string productId);
};
#endif




/**********************************************************************************************************************/
/* END OF FILE                                                                                                        */
/**********************************************************************************************************************/