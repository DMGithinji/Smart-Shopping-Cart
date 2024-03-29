#include <LiquidCrystal.h>            // Header inclusion
#include <Ethernet.h>
#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>

char j=0,x;
char Tag[13];
char item1[13]=   "12007AB87EAE";
char item2[13]= "120079A6CF02";
char item3[13]="4E0096039D46";
char item4[13]="4E00958F1347";
int price;
float total=0.00;
int count=0;
int m=0;
int n=0;
int o=0;
int p=0;
char sitem1[7]= "Router";
char sitem2[4]="Toy";
char sitem3[6]="Watch";
char sitem4[5]="Book";

byte mac_addr[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress server_addr(192,168,43,139);  // IP of the MySQL *server* here
char user[] = "esp8266";              // MySQL user login username
char password[] = "esp8266test";        // MySQL user login password

EthernetClient client;
MySQL_Connection conn((Client *)&client); 
 
char INSERT[]="INSERT INTO test_arduino.hello_sensor (Tag, Item, Price) VALUES ('%s','%s',%d)";
char DELETE[] = "DELETE FROM test_arduino.hello_sensor WHERE Tag='%s'"; 
char TRUNCATE[]="TRUNCATE TABLE test_arduino.hello_sensor";
char query[128]; 

const int rs = 9, en = 8, d4 = 5, d5 = 6, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() 
{
     lcd.begin(20, 4);                // Set up the LCD's number of columns and rows:
     lcd.setCursor(0,0);              // Position the LCD cursor
     lcd.print("RFID BASED SHOPPING");   // Print a message to the LCD.
     lcd.setCursor(0,1);              // Position the LCD cursor
     lcd.print("CART");
      
     //Serial.begin(9600);              // Initialize the serial communications:
     delay(1000);
     lcd.clear();
 
     Serial.begin(9600);
     while (!Serial); // wait for serial port to connect
     Ethernet.begin(mac_addr);
     Serial.println("Connecting...");
     lcd.print("Connecting...");
    
   
  if (conn.connect(server_addr, 3306, user, password)) {
    lcd.clear();
    delay(1000);   
}
  else
  {
    Serial.println("Connection failed.");
    lcd.clear();
    lcd.print("Connection failed.");
    conn.close();
  }

truncatetable();
     
}

void addition ()
{
total= total+price;
count= count+1;
lcd.setCursor(0,1);              
lcd.print("No. of items: ");
lcd.print(count); 
lcd.setCursor(0,2); 
lcd.print("Total cost");
lcd.setCursor(0,3);
lcd.print("Kshs "); 
lcd.print(total);    
}

void subtraction ()
{
total= total-price;
count= count-1;
lcd.setCursor(0,1);              // Position the LCD cursor
lcd.print("No. of items: ");
lcd.print(count); 
lcd.setCursor(0,2); 
lcd.print("Total cost");
lcd.setCursor(0,3);
lcd.print("Kshs "); 
lcd.print(total);     
}


void truncatetable()
{
    MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn);
    cur_mem->execute(TRUNCATE);
    delete cur_mem;
    Serial.println("Data recorded."); 
  }
  
void insertitem1()
{
  
               // Initiate the query class instance
    MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn);
    // Save

      sprintf(query, INSERT,Tag,sitem1,price,m);
            
    // Execute the query
    cur_mem->execute(query);
    // Note: since there are no results, we do not need to read any data
    // Deleting the cursor also frees up memory used
    delete cur_mem;
    Serial.println("Data recorded.");
   
  }

void insertitem2()
{
  
     // Initiate the query class instance
    MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn);
    // Save
   
      sprintf(query, INSERT,Tag,sitem2,price,n);      
    // Execute the query
    cur_mem->execute(query);
    // Note: since there are no results, we do not need to read any data
    // Deleting the cursor also frees up memory used
    delete cur_mem;
    Serial.println("Data recorded.");
  
  }

void insertitem3()
{
  
     // Initiate the query class instance
    MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn);
    // Save
   
      sprintf(query, INSERT,Tag,sitem3,price,o);      
    // Execute the query
    cur_mem->execute(query);
    // Note: since there are no results, we do not need to read any data
    // Deleting the cursor also frees up memory used
    delete cur_mem;
    Serial.println("Data recorded.");
  
  }

 void insertitem4()
{
  
     // Initiate the query class instance
    MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn);
    // Save
   
      sprintf(query, INSERT,Tag,sitem4,price,p);      
    // Execute the query
    cur_mem->execute(query);
    // Note: since there are no results, we do not need to read any data
    // Deleting the cursor also frees up memory used
    delete cur_mem;
    Serial.println("Data recorded.");
  }

 
void deleteitem1()
{

   // Initiate the query class instance
    MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn);
    // Save

     sprintf(query, DELETE,sitem1); 
         
    // Execute the query
    cur_mem->execute(query);
  
    delete cur_mem;
    Serial.println("Data recorded."); 
 
  }



 
void deleteitem2()
{
   // Initiate the query class instance
    MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn);
    // Save
    sprintf(query, DELETE,sitem2);        
    // Execute the query
    cur_mem->execute(query); 
    delete cur_mem;
    Serial.println("Data recorded."); 
  }

void deleteitem3()
{
   // Initiate the query class instance
    MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn);
    // Save
    sprintf(query, DELETE,sitem3);        
    // Execute the query
    cur_mem->execute(query); 
    delete cur_mem;
    Serial.println("Data recorded."); 
  }

void deleteitem4()
{
   // Initiate the query class instance
    MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn);
    // Save
    sprintf(query, DELETE,sitem4);        
    // Execute the query
    cur_mem->execute(query); 
    delete cur_mem;
    Serial.println("Data recorded."); 
  }

void loop() 
{ 
  if (Serial.available()>0) 
   
  {
    x=Serial.read();          
    Tag[j] =x;                    // Save the read_data to an array
    j++;

    if(j==12)                         // After intake of 12 character 
    { 
      lcd.clear(); 
      Tag[j]='\0';
      j=0;
      
      Serial.println(Tag);
      lcd.clear();
              
      if (strcmp(item1,Tag)== 0)
      {                               // Matching found in read_dat & card_id
         m=m+1;
         price=50;      

 if ( (m % 2) ==0)
          
          {
         
         lcd.setCursor(0,0);
         lcd.print(item1);
         lcd.print(" removed");
         subtraction();
         delay(1000);
         deleteitem1();
        
    
Serial.println(m);   
    
            }
      else 
            {

         lcd.setCursor(0,0);
         lcd.print(sitem1);
         lcd.print(" Kshs. "); 
         lcd.print(price); 
         addition();
         delay(1000);
         insertitem1();
        
 
          }
         
      }

       if (strcmp(item2,Tag)== 0)
      {                               // Matching found in read_dat & card_id
         n=n+1;
         price=100;

 if ( (n % 2) ==0)
          
          {
        
         lcd.setCursor(0,0);
         lcd.print(sitem2); 
         lcd.print(" removed");
         subtraction();
         delay(1000);
         deleteitem2();
        
     
            }
      else 
            {
         
         lcd.setCursor(0,0);
         lcd.print(sitem2); 
         lcd.print(" Kshs. ");
         lcd.print(price); 
         addition();
         delay(1000);
         insertitem2();
                 
          }         
      }

 if (strcmp(item3,Tag)== 0)
      {                               // Matching found in read_dat & card_id
         o=o+1;
         price=500;

 if ( (o % 2) ==0)
          
          {
        
         lcd.setCursor(0,0);
         lcd.print(sitem3); 
         lcd.print(" removed");
         subtraction();
         delay(1000);
         deleteitem3();
        
     
            }
      else 
            {
         
         lcd.setCursor(0,0);
         lcd.print(sitem3); 
         lcd.print(" Kshs. ");
         lcd.print(price); 
         addition();
         delay(1000);
         insertitem3();
                 
          }         
      }


 if (strcmp(item4,Tag)== 0)
      {                               // Matching found in read_dat & card_id
         p=p+1;
         price=300;

 if ( (p % 2) ==0)
          
          {
        
         lcd.setCursor(0,0);
         lcd.print(sitem4); 
         lcd.print(" removed");
         subtraction();
         delay(1000);
         deleteitem4();
        
     
            }
      else 
            {
         
         lcd.setCursor(0,0);
         lcd.print(sitem4); 
         lcd.print(" Kshs. ");
         lcd.print(price); 
         addition();
         delay(1000);
         insertitem4();
                 
          }         
      }
      
      else if ((strcmp(item1,Tag)!= 0) && (strcmp(item2,Tag)!= 0) && (strcmp(item3,Tag)!= 0) && (strcmp(item4,Tag)!= 0))                          // No matching found in read_dat & card_id
      
      {  
       
         lcd.setCursor(0,0); 
         lcd.print("Invalid code"); 
         delay(1000);
         Serial.println (Tag);
      }
      lcd.clear();
    }  
  }
  
  else                                // If not read data go to show the tag again
  {
     lcd.setCursor(0,0); 
     lcd.print("Place item to scan");
     lcd.setCursor(0,1);              // Position the LCD cursor
     lcd.print("No. of items: ");
     lcd.print(count); 
     lcd.setCursor(0,2); 
     lcd.print("Total cost");
     lcd.setCursor(0,3);
     lcd.print("Kshs "); 
     lcd.print(total);  
       
  }
  
}
