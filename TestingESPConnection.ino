/*
 * SimplePgSQL.c - Lightweight PostgreSQL connector for Arduino
 * Demo program: Simple PostgreSQL console over Serial
 *
 * - Accepts PostgreSQL simple queries from Serial
 * - \d            : displays table list
 * - \d <table>    : list table columns
 * - exit          : closes connection
 */


#include <WiFi.h>
#include <SimplePgSQL.h>
#include <ctype.h>
#include <string.h>


// -----------------------------------------------------------------------------
// WiFi & PostgreSQL configuration
// -----------------------------------------------------------------------------


// Your PostgreSQL server IP (replace with your actual server IP)
       
IPAddress PGIP(192, 168, 137, 1);


const char* ssid = "CARLALAPTOP 7629";
const char* password = "42R83=j8";


const char user[]    = "my_esp";     // database user
const char dbpass[]  = "pass";      // database password
const char dbname[]  = "myespdba";   // database name


WiFiClient client;


char buffer[1024];
PGconnection conn(&client, 0, 1024, buffer);


int WiFiStatus = WL_IDLE_STATUS;
int pg_status  = 0;


// -----------------------------------------------------------------------------
// SQL helper queries
// -----------------------------------------------------------------------------


static const char query_rel[] PROGMEM = "\
SELECT a.attname \"Column\",\
  pg_catalog.format_type(a.atttypid, a.atttypmod) \"Type\",\
  case when a.attnotnull then 'not null ' else 'null' end as \"null\",\
  (SELECT substring(pg_catalog.pg_get_expr(d.adbin, d.adrelid) for 128)\
   FROM pg_catalog.pg_attrdef d\
   WHERE d.adrelid = a.attrelid AND d.adnum = a.attnum AND a.atthasdef) \"Extras\"\
 FROM pg_catalog.pg_attribute a, pg_catalog.pg_class c\
 WHERE a.attrelid = c.oid AND c.relkind = 'r' AND\
 c.relname = %s AND\
 pg_catalog.pg_table_is_visible(c.oid)\
 AND a.attnum > 0 AND NOT a.attisdropped\
    ORDER BY a.attnum";


static const char query_tables[] PROGMEM = "\
SELECT n.nspname as \"Schema\",\
  c.relname as \"Name\",\
  CASE c.relkind WHEN 'r' THEN 'table' WHEN 'v' THEN 'view' WHEN 'm' THEN 'materialized view' WHEN 'i' THEN 'index' WHEN 'S' THEN 'sequence' WHEN 's' THEN 'special' WHEN 'f' THEN 'foreign table' END as \"Type\",\
  pg_catalog.pg_get_userbyid(c.relowner) as \"Owner\"\
 FROM pg_catalog.pg_class c\
     LEFT JOIN pg_catalog.pg_namespace n ON n.oid = c.relnamespace\
 WHERE c.relkind IN ('r','v','m','S','f','')\
      AND n.nspname <> 'pg_catalog'\
      AND n.nspname <> 'information_schema'\
      AND n.nspname !~ '^pg_toast'\
  AND pg_catalog.pg_table_is_visible(c.oid)\
 ORDER BY 1,2";


// -----------------------------------------------------------------------------
// WiFi helpers
// -----------------------------------------------------------------------------


void connectWiFi() {
    Serial.print("Connecting to WiFi SSID: ");
    Serial.println(ssid);


    WiFi.begin(ssid, password);


    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println();
    Serial.print("Connected. IP address: ");
    Serial.println(WiFi.localIP());


    WiFiStatus = WL_CONNECTED;
}


void checkConnection() {
    int status = WiFi.status();
    if (status != WiFiStatus) {
        WiFiStatus = status;
        if (status == WL_CONNECTED) {
            Serial.println("WiFi Connected");
            Serial.print("IP address: ");
            Serial.println(WiFi.localIP());
        } else {
            Serial.println("WiFi connection lost");
        }
    }
}


// -----------------------------------------------------------------------------
// PostgreSQL console logic
// -----------------------------------------------------------------------------


void doPg(void)
{
    char *msg;
    int rc;


    // Initial login setup
    if (!pg_status) {
        conn.setDbLogin(PGIP,
                        user,
                        dbpass,
                        dbname,
                        "utf8");
        pg_status = 1;
        return;
    }


    // Wait for connection to become OK
    if (pg_status == 1) {
        rc = conn.status();
        if (rc == CONNECTION_BAD || rc == CONNECTION_NEEDED) {
            char *c = conn.getMessage();
            if (c) Serial.println(c);
            pg_status = -1;
        }
        else if (rc == CONNECTION_OK) {
            pg_status = 2;
           
        }
        return;
    }


    // Ready to accept commands from Serial
    // After you have a connected `conn` and pg_status == 2
    if (pg_status == 2) {
        if (conn.execute("SELECT * FROM voltage_sensors;")) {
            goto error;
        }
        if (conn.execute("SELECT * FROM voltage_sensors;")) {
            goto error;
        }
        Serial.println("Working...");
        Serial.println(Serial.available());
        Serial.println(Serial1.available());
        Serial.println(Serial2.available());

        pg_status = 3;    // go to result-fetching state
        return;
    }


    // 3: fetch and print results
    if (pg_status == 3) {
        rc = conn.getData();


        if (rc < 0) goto error;
        if (!rc) return;  // nothing ready yet, come back next loop()


        if (rc & PG_RSTAT_HAVE_COLUMNS) {
            // print column names
            for (int i = 0; i < conn.nfields(); i++) {
                if (i) Serial.print(" | ");
                Serial.print(conn.getColumn(i));
            }
            Serial.println("\n==========");
        }
        else if (rc & PG_RSTAT_HAVE_ROW) {
            // print one row
            for (int i = 0; i < conn.nfields(); i++) {
                if (i) Serial.print(" | ");
                msg = conn.getValue(i);
                if (!msg) msg = (char*)"NULL";
                Serial.print(msg);
            }
            Serial.println();
        }
        else if (rc & PG_RSTAT_HAVE_SUMMARY) {
            Serial.print("Rows affected: ");
            Serial.println(conn.ntuples());
        }
        else if (rc & PG_RSTAT_HAVE_MESSAGE) {
            msg = conn.getMessage();
            if (msg) Serial.println(msg);
        }


        // done with this query
        if (rc & PG_RSTAT_READY) {
            Serial.println("Query finished.");
            pg_status = -1;   // or set to -1 if you only want it once
        }
        return;
    }


    return;


error:
    msg = conn.getMessage();
    if (msg) Serial.println(msg);
    else Serial.println("UNKNOWN ERROR");


    if (conn.status() == CONNECTION_BAD) {
        Serial.println("Connection is bad");
        pg_status = -1;
    }
}
// -----------------------------------------------------------------------------
// Arduino setup/loop
// -----------------------------------------------------------------------------
HardwareSerial SerialPortFromSTM32(1);


void setup(void)
{
    Serial.begin(115200); // USB to PC


    // STM32 UART: RX=16, TX=17, 115200 baud (match STM32)
    SerialPortFromSTM32.begin(115200, SERIAL_8N1, 16, 17);


    delay(1000);
    connectWiFi();
}

String line;

void loop() {
    checkConnection();

    while (SerialPortFromSTM32.available()) {
        char c = SerialPortFromSTM32.read();
        if (c == '\n') {
            Serial.print("Got line from STM32: ");
            Serial.println(line);
            // TODO: parse line into floats here
            line = "";
        } else if (c != '\r') {
            line += c;
        }
    }

    if (WiFi.status() == WL_CONNECTED && pg_status >= 0) {
        doPg();
    }

    delay(10);
}
