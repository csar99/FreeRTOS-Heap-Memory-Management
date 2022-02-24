/**
 * freeRTOS Heap memory example 
*/

#include <Arduino.h>

// Using only one core for demo purposes
#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

// creating a simple task
void simpleTask(void *parameter)
{
  while (1)
  {
    int a = 1;
    int b[100]; // size of an integer is 4 bytes. So total array size is, 4x100 = 400 bytes

    for (size_t i = 0; i < 100; i++)
    {
      b[i] = a + 1;
    }
    Serial.println(b[0]);

    // Print out remaining stack memory (words). 
    // Multiply this by 4 to get the availabele bytes in a 32 bit system
    Serial.print("High water mark (words): ");
    Serial.println(uxTaskGetStackHighWaterMark(NULL));

    // Print out number of free heap memory bytes before malloc
    Serial.print("Heap before malloc (bytes): ");
    Serial.println(xPortGetFreeHeapSize());

    int *ptr = (int *)pvPortMalloc(1024 * sizeof(int));

    // one way to prevent heap overflow is to check the malloc output
    if (ptr == NULL)
    {
      Serial.println("Not enough heap");
    }
    else
    {
      // Do something with the memory so it's not optimized out by the compiler
      for (size_t i = 0; i < 1024; i++)
      {
        ptr[i] = 3;
      }
    }

    // Print out number of free heap memory bytes after malloc
    Serial.print("Heap after malloc (bytes): ");
    Serial.println(xPortGetFreeHeapSize());

    // Free up our allocated memory
    vPortFree(ptr);

    // Wait for a while
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

void setup()
{
  // Configure serial
  Serial.begin(115200);

  // Add a delay for start
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  Serial.println();
  Serial.println("-----FREERTOS MEMORY MANAGEMENT-----");

  // Start only the simpleTask()
  xTaskCreatePinnedToCore(simpleTask,
                          "Simple Task",
                          1500, // Stack size, 1024 too small, instead use a higher value
                          NULL,
                          1,
                          NULL,
                          app_cpu);

  // Delelte the setup and loop tasks
  vTaskDelete(NULL);
}

void loop()
{
  // put your main code here, to run repeatedly:
}