#define WIFI_SSID       "CHANGE_ME"
#define WIFI_PASS       "CHANGE_ME"

// the group will be used as a prefix for the feeds.
// the feeds you need to create are "color", "brightness", "white" under this group.
#define IO_GROUP       "CHANGE_ME"
#define IO_USERNAME    "CHANGE_ME"
#define IO_KEY         "CHANGE_ME"

// Something unique so you can easily recognize this device
#define OTA_IDENTIFIER  "CHANGE_ME"
// Can be also left blank.
#define OTA_PASSWORD    "CHANGE_ME"

// Connect your led strip only using PWM pins, configure those below.
// If you don't have white color, just set it to an empty PWM pin. (do not omit!)
RGBW pin = { 2, 0, 4, 5 };
