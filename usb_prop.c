#include "usb_lib.h" 
#include "usb_conf.h"
#include "usb_prop.h"
#include "usb_desc.h"
#include "usb_pwr.h"

uint32_t ProtocolValue = 0;

// ===========================================================================
// 描述符封装
// ===========================================================================
ONE_DESCRIPTOR Device_Descriptor = {
  (uint8_t*)Mouse_DeviceDescriptor,
  MOUSE_SIZ_DEVICE_DESC
};

ONE_DESCRIPTOR Config_Descriptor = {
  (uint8_t*)Mouse_ConfigDescriptor,
  MOUSE_SIZ_CONFIG_DESC
};

ONE_DESCRIPTOR Report_Descriptor = {
  (uint8_t*)Mouse_ReportDescriptor,
  MOUSE_SIZ_REPORT_DESC
};

ONE_DESCRIPTOR String_Descriptor[4] = {
  { (uint8_t*)Mouse_StringLangID, MOUSE_SIZ_STRING_LANGID },
  { (uint8_t*)Mouse_StringVendor, MOUSE_SIZ_STRING_VENDOR },
  { (uint8_t*)Mouse_StringProduct, MOUSE_SIZ_STRING_PRODUCT },
  { (uint8_t*)Mouse_StringSerial, MOUSE_SIZ_STRING_SERIAL }
};

ONE_DESCRIPTOR Mouse_Hid_Descriptor = {
  (uint8_t*)Mouse_ConfigDescriptor + 18,
  0x09
};

// ===========================================================================
// 注册表 (关键修正：类型名称是 DEVICE)
// ===========================================================================

DEVICE Device_Table = { EP_NUM, 1 };

DEVICE_PROP Device_Property = {
  Mouse_Init,
  Mouse_Reset,
  Mouse_Status_In,
  Mouse_Status_Out,
  Mouse_Data_Setup,
  Mouse_NoData_Setup,
  Mouse_Get_Interface_Setting,
  Mouse_GetDeviceDescriptor,
  Mouse_GetConfigDescriptor,
  Mouse_GetStringDescriptor,
  0,
  0x40 
};

// 关键修正：这里的函数签名现在与 usb_core.h 中的定义完全匹配 (void)
USER_STANDARD_REQUESTS User_Standard_Requests = {
  Mouse_GetConfiguration,
  Mouse_SetConfiguration,
  Mouse_GetInterface,
  Mouse_SetInterface,
  Mouse_GetStatus,
  Mouse_ClearFeature,
  Mouse_SetEndPointFeature,
  Mouse_SetDeviceFeature,
  Mouse_SetDeviceAddress
};

// ===========================================================================
// 函数实现
// ===========================================================================

void Mouse_Init(void)
{
  pInformation->Current_Configuration = 0;
  PowerOn();
  USB_SIL_Init();
  bDeviceState = UNCONNECTED;
}

void Mouse_Reset(void)
{
  pInformation->Current_Configuration = 0;
  pInformation->Current_Interface = 0;
  SetBTABLE(BTABLE_ADDRESS);

  SetEPType(ENDP0, EP_CONTROL);
  SetEPTxStatus(ENDP0, EP_TX_STALL);
  SetEPRxAddr(ENDP0, ENDP0_RXADDR);
  SetEPTxAddr(ENDP0, ENDP0_TXADDR);
  Clear_Status_Out(ENDP0);
  SetEPRxCount(ENDP0, Device_Property.MaxPacketSize);
  SetEPRxValid(ENDP0);

  SetEPType(ENDP1, EP_INTERRUPT);
  SetEPTxAddr(ENDP1, ENDP1_TXADDR);
  SetEPTxCount(ENDP1, 4);
  SetEPRxStatus(ENDP1, EP_RX_DIS);
  SetEPTxStatus(ENDP1, EP_TX_NAK);

  bDeviceState = ATTACHED;
  SetDeviceAddress(0);
}

void Mouse_SetConfiguration(void)
{
  if (pInformation->Current_Configuration != 0)
  {
    bDeviceState = CONFIGURED;
  }
}

void Mouse_SetDeviceAddress (void)
{
  bDeviceState = ADDRESSED;
}

void Mouse_Status_In(void) {}
void Mouse_Status_Out(void) {}

RESULT Mouse_Data_Setup(uint8_t RequestNo)
{
  uint8_t *(*CopyRoutine)(uint16_t);
  CopyRoutine = NULL;

  if ((RequestNo == GET_DESCRIPTOR) && (Type_Recipient == (STANDARD_REQUEST | INTERFACE_RECIPIENT)) && (pInformation->USBwIndex0 == 0))
  {
      if (pInformation->USBwValue1 == REPORT_DESCRIPTOR)
      {
        CopyRoutine = Mouse_GetReportDescriptor;
      }
      else if (pInformation->USBwValue1 == HID_DESCRIPTOR_TYPE)
      {
        CopyRoutine = Mouse_GetHIDDescriptor;
      }
  }
  else if ((Type_Recipient == (CLASS_REQUEST | INTERFACE_RECIPIENT)) && (RequestNo == GET_PROTOCOL))
  {
    CopyRoutine = Mouse_GetProtocolValue;
  }

  if (CopyRoutine == NULL) return USB_UNSUPPORT;

  pInformation->Ctrl_Info.CopyData = CopyRoutine;
  pInformation->Ctrl_Info.Usb_wOffset = 0;
  (*CopyRoutine)(0);
  return USB_SUCCESS;
}

RESULT Mouse_NoData_Setup(uint8_t RequestNo)
{
  if ((Type_Recipient == (CLASS_REQUEST | INTERFACE_RECIPIENT)) && (RequestNo == SET_PROTOCOL))
  {
    return Mouse_SetProtocol();
  }
  return USB_UNSUPPORT;
}

uint8_t *Mouse_GetDeviceDescriptor(uint16_t Length)
{
  return Standard_GetDescriptorData(Length, &Device_Descriptor);
}

uint8_t *Mouse_GetConfigDescriptor(uint16_t Length)
{
  return Standard_GetDescriptorData(Length, &Config_Descriptor);
}

uint8_t *Mouse_GetStringDescriptor(uint16_t Length)
{
  uint8_t wValue0 = pInformation->USBwValue0;
  if (wValue0 > 4) return NULL;
  return Standard_GetDescriptorData(Length, &String_Descriptor[wValue0]);
}

uint8_t *Mouse_GetReportDescriptor(uint16_t Length)
{
  return Standard_GetDescriptorData(Length, &Report_Descriptor);
}

uint8_t *Mouse_GetHIDDescriptor(uint16_t Length)
{
  return Standard_GetDescriptorData(Length, &Mouse_Hid_Descriptor);
}

RESULT Mouse_SetProtocol(void)
{
  ProtocolValue = (uint8_t)(pInformation->USBwValue0);
  return USB_SUCCESS;
}

uint8_t *Mouse_GetProtocolValue(uint16_t Length)
{
  if (Length == 0)
  {
    pInformation->Ctrl_Info.Usb_wLength = 1;
    return NULL;
  }
  return (uint8_t *)(&ProtocolValue);
}

RESULT Mouse_Get_Interface_Setting(uint8_t Interface, uint8_t AlternateSetting)
{
  if (AlternateSetting > 0) return USB_UNSUPPORT;
  else if (Interface > 0) return USB_UNSUPPORT;
  return USB_SUCCESS;
}

// 关键修正：标准请求桩函数全部改为 void 类型，且无返回值
void Mouse_GetConfiguration(void) {}
void Mouse_GetInterface(void) {}
void Mouse_SetInterface(void) {}
void Mouse_GetStatus(void) {}
void Mouse_ClearFeature(void) {}
void Mouse_SetEndPointFeature(void) {}
void Mouse_SetDeviceFeature(void) {}
