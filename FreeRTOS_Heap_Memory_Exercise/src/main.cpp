/**
 * Heap Memory Exercise 
 * Create two tasks, task A and task B
 * 
 * Task A should listen to the inputs in serial monitor.
 * On new line input ('/n') it should store all the characters upto that point
 * in the heap memory.
 * Then it notifies task B of new message
 * 
 * Task B should waits for notification from task A
 * Then it should print mesage found in heap memory to Serial monitor
 * Finally it should free the heap memory.
*/
#include <Arduino.h>

#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

// String variable for get the input
String user_input;
char *chrPtr;
char userInputArr[100];

// Bool variable to notify
bool isReady = false;

// Task A
void task_A(void *parameter)
{
  while (1)
  {
    
    // Print out remaining stack memory (words).
    // Multiply this by 4 to get the availabele bytes in a 32 bit system, 8 for 64 bit system
     Serial.print("High water mark (bytes): ");
     Serial.println(uxTaskGetStackHighWaterMark(NULL) * 8);

     Serial.print("Stack size before: ");
     Serial.println(xPortGetFreeHeapSize());

    while (Serial.available())
    {
      user_input = Serial.readString();

      for (size_t i = 0; i < user_input.length(); i++)
      {
        userInputArr[i] = user_input[i];
      }
      chrPtr = userInputArr;
      chrPtr = (char *)pvPortMalloc(1024 * sizeof(char));

      for (size_t i = 0; i < 100; i++)
      {
        Serial.print(userInputArr[i]);
      }
      Serial.println();

      // release memory
      vPortFree(chrPtr);
      // Serial.println(user_input);
      vTaskDelay(2000 / portTICK_PERIOD_MS);

      isReady = true;
    }
    
     Serial.print("Stack size after: ");
     Serial.println(xPortGetFreeHeapSize());

    vTaskDelay(200 / portTICK_PERIOD_MS);

    // free up the allocated memory
  }
}

 void task_B(void *parameter)
 {
   while (1)
  {
    //Serial.println("Task b is running");

    if (isReady == true)
    {
      // Serial.println("In Task B's if statement");
      // Serial.println(user_input);

      vPortFree(chrPtr);

      isReady = false;

      vTaskDelay(200 / portTICK_PERIOD_MS);
    }
  }
}

void setup()
{
  Serial.begin(115200);

  vTaskDelay(2000 / portTICK_PERIOD_MS);

  xTaskCreatePinnedToCore(task_A,
                          "Task A",
                          1024,
                          NULL,
                          1,
                          NULL,
                          app_cpu);

   xTaskCreatePinnedToCore(task_B,
                          "Task B",
                           1024,
                           NULL,
                           0,
                           NULL,
                           app_cpu);


  vTaskDelete(NULL);
}

void loop()
{
  // put your main code here, to run repeatedly:
}