//
// Created by indrek on 1.05.2016.
//

// set EXAMPLE to EXAMPLE_UART in setup.h to activate
#include "setup.h"
#if EXAMPLE == 3
#include "Arduino.h"
#include "CameraOV7670.h"



// select resolution and communication speed:
// 1 - 320x240 with 2M baud
// 2 - 320x240 with 1M baud
// 3 - 160x120 with 1M baud
// 4 - 160x120 with 115200 baud
// 5 - 320x240 grayscale with 1M baud
// 6 - 160x120 grayscale with 1M baud
// 7 - 640x480 grayscale with 1M baud 
// 8 - 640x480 grayscale with 2M baud
// 9 - 640x480 with 2M baud
#define UART_MODE 2



static const uint8_t VERSION = 0x00;
static const uint8_t COMMAND_NEW_FRAME = 0x01 | VERSION;
static const uint8_t COMMAND_END_OF_LINE = 0x02;
static const uint8_t COMMAND_DEBUG_DATA = 0x03;

static const uint16_t COLOR_GREEN = 0x07E0;
static const uint16_t COLOR_RED = 0xF800;

// Lower three bits 0b00000111.
// Upper bits reserved for future attributes.
static const uint16_t UART_PIXEL_BYTE_PARITY_CHECK = 0x80;
static const uint16_t UART_PIXEL_FORMAT_RGB565 = 0x01 | UART_PIXEL_BYTE_PARITY_CHECK; // Signal to ArduImageCapture that parity check is enabled
static const uint16_t UART_PIXEL_FORMAT_GRAYSCALE = 0x02;


// Pixel byte parity check:
// Pixel Byte H: odd number of bits under H_BYTE_PARITY_CHECK and H_BYTE_PARITY_INVERT
// Pixel Byte L: even number of bits under L_BYTE_PARITY_CHECK and L_BYTE_PARITY_INVERT
//                                          H:RRRRRGGG
static const uint8_t H_BYTE_PARITY_CHECK =  0b00100000;
static const uint8_t H_BYTE_PARITY_INVERT = 0b00001000;
//                                          L:GGGBBBBB
static const uint8_t L_BYTE_PARITY_CHECK =  0b00001000;
static const uint8_t L_BYTE_PARITY_INVERT = 0b00100000;
// Since the parity for L byte can be zero we must ensure that the total byet value is above zero.
// Increasing the lowest bet of blue color is OK for that.
static const uint8_t L_BYTE_PREVENT_ZERO  = 0b00000001;



#if UART_MODE==1
static const uint16_t lineLength = 320;
static const uint16_t lineCount = 240;
static const uint32_t baud  = 2000000; // may be unreliable
static const bool isBufferedLine = true;
static const bool isSendWhileBuffering = true;
static const uint8_t uartPixelFormat = UART_PIXEL_FORMAT_RGB565;
CameraOV7670 camera(CameraOV7670::RESOLUTION_QVGA_320x240, CameraOV7670::PIXEL_RGB565, 11);
#endif

#if UART_MODE==2
static const uint16_t lineLength = 320;
static const uint16_t lineCount = 240;
static const uint32_t baud  = 1000000;
static const bool isBufferedLine = true;
static const bool isSendWhileBuffering = true;
static const uint8_t uartPixelFormat = UART_PIXEL_FORMAT_RGB565;
CameraOV7670 camera(CameraOV7670::RESOLUTION_QVGA_320x240, CameraOV7670::PIXEL_RGB565, 16);
#endif

#if UART_MODE==3
static const uint16_t lineLength = 160;
static const uint16_t lineCount = 120;
static const uint32_t baud  = 1000000;
static const bool isBufferedLine = true;
static const bool isSendWhileBuffering = false;
static const uint8_t uartPixelFormat = UART_PIXEL_FORMAT_RGB565;
CameraOV7670 camera(CameraOV7670::RESOLUTION_QQVGA_160x120, CameraOV7670::PIXEL_RGB565, 5);
#endif

#if UART_MODE==4
static const uint16_t lineLength = 160;
static const uint16_t lineCount = 120;
static const uint32_t baud  = 115200;
static const bool isBufferedLine = true;
static const bool isSendWhileBuffering = true;
static const uint8_t uartPixelFormat = UART_PIXEL_FORMAT_RGB565;
CameraOV7670 camera(CameraOV7670::RESOLUTION_QQVGA_160x120, CameraOV7670::PIXEL_RGB565, 34);
#endif

#if UART_MODE==5
static const uint16_t lineLength = 320;
static const uint16_t lineCount = 240;
static const uint32_t baud  = 1000000;
static const bool isBufferedLine = true;
static const bool isSendWhileBuffering = true;
static const uint8_t uartPixelFormat = UART_PIXEL_FORMAT_GRAYSCALE;
CameraOV7670 camera(CameraOV7670::RESOLUTION_QVGA_320x240, CameraOV7670::PIXEL_YUV422, 8);
#endif

#if UART_MODE==6
static const uint16_t lineLength = 160;
static const uint16_t lineCount = 120;
static const uint32_t baud  = 1000000;
static const bool isBufferedLine = true;
static const bool isSendWhileBuffering = false;
static const uint8_t uartPixelFormat = UART_PIXEL_FORMAT_GRAYSCALE;
CameraOV7670 camera(CameraOV7670::RESOLUTION_QQVGA_160x120, CameraOV7670::PIXEL_YUV422, 2);
#endif

#if UART_MODE==7
static const uint16_t lineLength = 640;
static const uint16_t lineCount = 480;
static const uint32_t baud  = 1000000;
static const bool isBufferedLine = false;
static const bool isSendWhileBuffering = false;
static const uint8_t uartPixelFormat = UART_PIXEL_FORMAT_GRAYSCALE;
CameraOV7670 camera(CameraOV7670::RESOLUTION_VGA_640x480, CameraOV7670::PIXEL_YUV422, 39);
#endif

#if UART_MODE==8
static const uint16_t lineLength = 640;
static const uint16_t lineCount = 480;
static const uint32_t baud  = 2000000;
static const bool isBufferedLine = false;
static const bool isSendWhileBuffering = false;
static const uint8_t uartPixelFormat = UART_PIXEL_FORMAT_GRAYSCALE;
CameraOV7670 camera(CameraOV7670::RESOLUTION_VGA_640x480, CameraOV7670::PIXEL_YUV422, 19);
#endif

#if UART_MODE==9
static const uint16_t lineLength = 640;
static const uint16_t lineCount = 480;
static const uint32_t baud  = 2000000;
static const bool isBufferedLine = false;
static const bool isSendWhileBuffering = false;
static const uint8_t uartPixelFormat = UART_PIXEL_FORMAT_RGB565;
CameraOV7670 camera(CameraOV7670::RESOLUTION_VGA_640x480, CameraOV7670::PIXEL_RGB565, 39);
#endif


uint8_t lineBuffer [isBufferedLine ? lineLength * 2 : 1]; // Two bytes per pixel
uint8_t * lineBufferSendByte;
bool isLineBufferSendHighByte;
bool isLineBufferByteFormatted;

uint16_t frameCounter = 0;
uint16_t processedByteCountDuringCameraRead = 0;



void sendBlankFrame(uint16_t color);
void processGrayscaleFrameBuffered();
void processGrayscaleFrameDirect();
void processRgbFrameBuffered();
void processRgbFrameDirect();
inline void startNewFrame(uint8_t pixelFormat) __attribute__((always_inline));
inline void endOfLine(void) __attribute__((always_inline));
inline void processNextGrayscalePixelByteInBuffer() __attribute__((always_inline));
inline void processNextRgbPixelByteInBuffer() __attribute__((always_inline));
inline void tryToSendNextRgbPixelByteInBuffer() __attribute__((always_inline));
inline void formatNextRgbPixelByteInBuffer() __attribute__((always_inline));
inline uint8_t formatRgbPixelByteH(uint8_t byte) __attribute__((always_inline));
inline uint8_t formatRgbPixelByteL(uint8_t byte) __attribute__((always_inline));
inline uint8_t formatPixelByteGrayscale(uint8_t byte) __attribute__((always_inline));
inline void waitForPreviousUartByteToBeSent() __attribute__((always_inline));
inline bool isUartReady() __attribute__((always_inline));

inline void debugPrint(const String debugText) __attribute__((always_inline));


// this is called in Arduino setup() function
void initializeScreenAndCamera() {

  // Enable this for WAVGAT CPUs
  // For UART communiation we want to set WAVGAT Nano to 16Mhz to match Atmel based Arduino
  //CLKPR = 0x80; // enter clock rate change mode
  //CLKPR = 1; // set prescaler to 1. WAVGAT MCU has it 3 by default.

  Serial.begin(baud);
  if (camera.init()) {
    sendBlankFrame(COLOR_GREEN);
    delay(1000);
  } else {
    sendBlankFrame(COLOR_RED);
    delay(3000);
  }
}


void sendBlankFrame(uint16_t color) {
  uint8_t colorH = (color >> 8) & 0xFF;
  uint8_t colorL = color & 0xFF;

  startNewFrame(UART_PIXEL_FORMAT_RGB565);
  for (uint16_t j=0; j<lineCount; j++) {
    for (uint16_t i=0; i<lineLength; i++) {
      waitForPreviousUartByteToBeSent();
      UDR0 = formatRgbPixelByteH(colorH);
      waitForPreviousUartByteToBeSent();
      UDR0 = formatRgbPixelByteL(colorL);
    }
    endOfLine();
  }
}




// this is called in Arduino loop() function
void processFrame() {
  processedByteCountDuringCameraRead = 0;
  startNewFrame(uartPixelFormat);
  noInterrupts();
    
  if (uartPixelFormat == UART_PIXEL_FORMAT_GRAYSCALE) {
    if (isBufferedLine) {
      processGrayscaleFrameBuffered();
    } else {
      processGrayscaleFrameDirect();
    }
  } else {
    if (isBufferedLine) {
      processRgbFrameBuffered();
    } else {
      processRgbFrameDirect();
    }
    
  }

  interrupts();
  frameCounter++;
  debugPrint("Frame " + String(frameCounter) + " " + String(processedByteCountDuringCameraRead));
  //debugPrint("Frame " + String(frameCounter, 16)); // send number in hexadecimal
}


void processGrayscaleFrameBuffered() {
  camera.waitForVsync();
  camera.ignoreVerticalPadding();

  for (uint16_t y = 0; y < lineCount; y++) {
    lineBufferSendByte = &lineBuffer[0];
    camera.ignoreHorizontalPaddingLeft();

    for (uint16_t x = 0; x < lineLength; x++) {
      camera.waitForPixelClockRisingEdge(); // YUV422 grayscale byte
      camera.readPixelByte(lineBuffer[x]);
      lineBuffer[x] = formatPixelByteGrayscale(lineBuffer[x]);

      camera.waitForPixelClockRisingEdge(); // YUV422 color byte. Ignore.
      if (isSendWhileBuffering) {
        processNextGrayscalePixelByteInBuffer();
      }
    }
    camera.ignoreHorizontalPaddingRight();

    // Debug info to get some feedback how mutch data was processed during line read.
    processedByteCountDuringCameraRead = lineBufferSendByte - (&lineBuffer[0]);

    // Send rest of the line
    while (lineBufferSendByte < &lineBuffer[lineLength]) {
      processNextGrayscalePixelByteInBuffer();
    }
 
    endOfLine();
  };
}

void processNextGrayscalePixelByteInBuffer() {
  if (isUartReady()) {
    UDR0 = *lineBufferSendByte;
    lineBufferSendByte++;        
  }
}



void processGrayscaleFrameDirect() {
  camera.waitForVsync();
  camera.ignoreVerticalPadding();
  
  for (uint16_t y = 0; y < lineCount; y++) {
    camera.ignoreHorizontalPaddingLeft();

    for (uint16_t x = 0; x < lineLength; x++) {
      camera.waitForPixelClockRisingEdge(); // YUV422 grayscale byte
      camera.readPixelByte(lineBuffer[0]);
      lineBuffer[0] = formatPixelByteGrayscale(lineBuffer[0]);
      
      camera.waitForPixelClockRisingEdge(); // YUV422 color byte. Ignore.
      waitForPreviousUartByteToBeSent();
      UDR0 = lineBuffer[0];
    }

    camera.ignoreHorizontalPaddingRight();
    endOfLine();
  }
}

uint8_t formatPixelByteGrayscale(uint8_t pixelByte) {
  return pixelByte | 0b00000001; // make pixel color always slightly above 0 since zero is end of line marker
}




void processRgbFrameBuffered() {
  camera.waitForVsync();
  camera.ignoreVerticalPadding();
  
  for (uint16_t y = 0; y < lineCount; y++) {
    lineBufferSendByte = &lineBuffer[0];
    isLineBufferSendHighByte = true; // Line starts with High byte
    isLineBufferByteFormatted = false;

    camera.ignoreHorizontalPaddingLeft();

    for (uint16_t x = 0; x < lineLength * 2;  x++) {
      camera.waitForPixelClockRisingEdge();
      camera.readPixelByte(lineBuffer[x]);
      if (isSendWhileBuffering) {
        processNextRgbPixelByteInBuffer();
      }
    };

    camera.ignoreHorizontalPaddingRight();

    // Debug info to get some feedback how mutch data was processed during line read.
    processedByteCountDuringCameraRead = lineBufferSendByte - (&lineBuffer[0]);

    // send rest of the line
    while (lineBufferSendByte < &lineBuffer[lineLength * 2]) {
      processNextRgbPixelByteInBuffer();
    }

    endOfLine();
  }
}

void processNextRgbPixelByteInBuffer() {
  // Format pixel bytes and send out in different cycles.
  // There is not enough time to do both on faster frame rates.
  if (isLineBufferByteFormatted) {
    tryToSendNextRgbPixelByteInBuffer();
  } else {
    formatNextRgbPixelByteInBuffer();
  }        
}

void tryToSendNextRgbPixelByteInBuffer() {
  if (isUartReady()) {
    UDR0 = *lineBufferSendByte;
    lineBufferSendByte++;    
    isLineBufferByteFormatted = false;    
  }
}

void formatNextRgbPixelByteInBuffer() {
  if (isLineBufferSendHighByte) {
    *lineBufferSendByte = formatRgbPixelByteH(*lineBufferSendByte);
  } else {
    *lineBufferSendByte = formatRgbPixelByteL(*lineBufferSendByte);
  }
  isLineBufferByteFormatted = true;
  isLineBufferSendHighByte = !isLineBufferSendHighByte;
}




void processRgbFrameDirect() {
  camera.waitForVsync();
  camera.ignoreVerticalPadding();

  for (uint16_t y = 0; y < lineCount; y++) {
    camera.ignoreHorizontalPaddingLeft();
    
    for (uint16_t x = 0; x < lineLength; x++) {
      
      camera.waitForPixelClockRisingEdge();
      camera.readPixelByte(lineBuffer[0]);
      lineBuffer[0] = formatRgbPixelByteH(lineBuffer[0]);
      waitForPreviousUartByteToBeSent();
      UDR0 = lineBuffer[0];
      
      camera.waitForPixelClockRisingEdge();
      camera.readPixelByte(lineBuffer[0]);
      lineBuffer[0] = formatRgbPixelByteL(lineBuffer[0]);
      waitForPreviousUartByteToBeSent();
      UDR0 = lineBuffer[0];
    }
    
    camera.ignoreHorizontalPaddingRight();
    endOfLine();
  };
}


// RRRRRGGG
uint8_t formatRgbPixelByteH(uint8_t pixelByteH) {
  // Make sure that
  // A: pixel color always slightly above 0 since zero is end of line marker
  // B: odd number of bits for H byte under H_BYTE_PARITY_CHECK and H_BYTE_PARITY_INVERT to enable error correction
  if (pixelByteH & H_BYTE_PARITY_CHECK) {
    return pixelByteH & (~H_BYTE_PARITY_INVERT);
  } else {
    return pixelByteH | H_BYTE_PARITY_INVERT;
  }
}


// GGGBBBBB
uint8_t formatRgbPixelByteL(uint8_t pixelByteL) {
  // Make sure that
  // A: pixel color always slightly above 0 since zero is end of line marker
  // B: even number of bits for L byte under L_BYTE_PARITY_CHECK and L_BYTE_PARITY_INVERT to enable error correction
  if (pixelByteL & L_BYTE_PARITY_CHECK) {
    return pixelByteL | L_BYTE_PARITY_INVERT | L_BYTE_PREVENT_ZERO;
  } else {
    return (pixelByteL & (~L_BYTE_PARITY_INVERT)) | L_BYTE_PREVENT_ZERO;
  }
}









void startNewFrame(uint8_t pixelFormat) {
  waitForPreviousUartByteToBeSent();
  UDR0 = 0x00;
  waitForPreviousUartByteToBeSent();
  UDR0 = COMMAND_NEW_FRAME;

  // frame width
  waitForPreviousUartByteToBeSent();
  UDR0 = (lineLength >> 8) & 0xFF;
  waitForPreviousUartByteToBeSent();
  UDR0 = lineLength & 0xFF;

  // frame height
  waitForPreviousUartByteToBeSent();
  UDR0 = (lineCount >> 8) & 0xFF;
  waitForPreviousUartByteToBeSent();
  UDR0 = lineCount & 0xFF;

  // pixel format
  waitForPreviousUartByteToBeSent();
  UDR0 = (pixelFormat);
}



void endOfLine()   {
  waitForPreviousUartByteToBeSent();
  UDR0 = 0x00;
  waitForPreviousUartByteToBeSent();
  UDR0 = COMMAND_END_OF_LINE;
}



void debugPrint(const String debugText) {
    waitForPreviousUartByteToBeSent();
    UDR0 = 0x00;
    waitForPreviousUartByteToBeSent();
    UDR0 = COMMAND_DEBUG_DATA;
    waitForPreviousUartByteToBeSent();
    UDR0 = debugText.length();
    for (uint16_t i=0; i<debugText.length(); i++) {
        waitForPreviousUartByteToBeSent();
        UDR0 = debugText[i];
    }
}



void waitForPreviousUartByteToBeSent() {
  while(!isUartReady()); //wait for byte to transmit
}


bool isUartReady() {
  return UCSR0A & (1<<UDRE0);
}


#endif
