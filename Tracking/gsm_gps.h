//function prototypes

void gsm_gps_init(void);
void send_char(char data);
void send_string(char *str);
char get_char(void);
void get_string(char *buffer, unsigned short length);

void send_char1(char data);
void send_string1(char *str);
char get_char1(void);
void get_string1(char *buffer, unsigned short length);

void receive_gps(void);
void sendsms(void);
void delayfunc() ;
void sendstring(unsigned char *p) ;
//void send_location();
void print_int(unsigned int integer);
void sendstring1(unsigned char *p);
void send_location(unsigned char *gps,unsigned short gps_dat);