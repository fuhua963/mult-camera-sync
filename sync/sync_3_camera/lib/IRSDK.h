#ifndef _IRSDK_H
#define _IRSDK_H

#ifdef _WIN32
#include <stdio.h>
#define IR_SDK_API  extern "C" __declspec(dllexport) 
#else
#define IR_SDK_API  extern "C"
#endif

#define DEVICE_MAX				(32)							//���֧������32���豸��ʵ�����
#define OBJ_MAX					(32)							//���֧������32�������ϲ�ֻ������32�����ܳ���

typedef int (*CBF_IR)(void * lData, void * lParam);				//SDK�лص������ĸ�ʽ����

//�¶�ת��
#define CALTEMP(x,y)		((x-10000)/(float)y)				//Y16�������¶����ݵ�ת����ϵ

//�ļ�����
#define OPEN_FILE				(1)
#define CLOSE_FILE				(2)
#define WR_FRAME 				(3)

//�۽�����
#define  STOPFOCUS				(0)
#define  FARFOCUS				(1)
#define  NEARFOCUS				(2)
#define  AUTOFOCUS				(3)
#define  FOCUSSTATUS			(4)

#define  MAX_W					(2048)
#define  MAX_H					(1536)

//֡��ʽ,����ǰ��32Byte֡ͷ
typedef struct tagFrame
{
	unsigned short width;				//ͼ����	
	unsigned short height;				//ͼ��߶�
	unsigned short u16FpaTemp;			//�����¶�
	unsigned short u16EnvTemp;			//�����¶�
	unsigned char  u8TempDiv;			//����ת�����¶�ʱ��Ҫ���Ը����ݣ��ĵ�����˵��
	unsigned char  u8DeviceType;		//unused
	unsigned char  u8SensorType;		//unused
	unsigned char  u8MeasureSel;		//���¶�
	unsigned char  u8Lens;				//��ͷ
	unsigned char  u8Fps;				//֡��
	unsigned char  u8TriggerFrame;		//����֡
	unsigned char  u8Reversed2;			//unused 
	unsigned int   u32FrameIndex;		//֡����
	unsigned short u16MeasureDis;		//����
	unsigned char  Reversed[8];		    //unused
	unsigned char  u8Handle;		    //���
	unsigned char  u8ObjTempFilterSw;	//��ȡ�¶��˲�����
	unsigned short buffer[MAX_W*MAX_H];		//ͼ�����ݣ����д洢�����֧�� 1280x1024��ÿ��������һ�� ushort ����
} Frame;
 

//�㣬 ��x,y)Ϊ����
typedef struct t_point
{
	unsigned short x;
	unsigned short y;
}T_POINT;

//�ߣ�P1,P2Ϊ���˵�
typedef struct t_line
{
	T_POINT P1;
	T_POINT P2;
}T_LINE;

//Բ��֧����Բ����Pc��ʾԲ�����꣬a��ʾ����ᣬb��ʾ�����ᣬ����Բ��a=b����ʾ�뾶
typedef struct t_circle
{
	T_POINT Pc;
	unsigned short a;
	unsigned short b;
}T_CIRCLE;

//���Σ� P1Ϊ�������϶������꣬P2Ϊ���¶�������
typedef struct t_rect
{
	T_POINT P1;
	T_POINT P2;
}T_RECT;

//�������Σ����֧��16�����㣬Pt_numΪ���������PtΪÿ���������
typedef struct t_polygon
{
	unsigned int Pt_num;
	T_POINT Pt[16];
}T_POLYGON;

//�״�ͼ�����֧��64�����㣬Pt_numΪ���������PtΪÿ���������
typedef struct t_radar
{
	unsigned int Pt_num;
	unsigned char circle_num;
	unsigned short max_radiu;
	T_POINT Pt[64];
}T_RADAR;

//�����Сƽ���¶ȼ���ֵ��Ӧ����
typedef struct stat_temper
{
	float maxTemper;
	float minTemper;
	float avgTemper;

	T_POINT maxTemperPT;
	T_POINT minTemperPT;
}STAT_TEMPER;

//��ɫ
typedef struct t_color
{
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;
}T_COLOR;

//������������
enum T_ALARMTYPE
{
	OverHigh	= 0,			//���ڸ�����
	UnderLow	= 1,			//���ڵ�����
	BetweenHL	= 2,			//����
	DeBetweenHL = 3,			//��ѡ����
};

//�澯
typedef struct t_alarm
{
	unsigned char alarmType;	//�澯����
	unsigned char isDraw;		//�Ƿ�����
	unsigned char isVioce;		//�Ƿ������澯
	unsigned char isVideo;		//�Ƿ�¼��
	float		  HighThresh;	//������
	float		  LowThresh;	//������
	T_COLOR		  colorAlarm; 	//�澯��ɫ	
}T_ALARM;

//ȫ���¶�
typedef struct stat_global
{
	stat_global()
	{
		inputEmiss = 0.98;
		inputReflect = 20.0;
		inputDis = 2.0;
		inputOffset = 0;
	}
	STAT_TEMPER sTemp;
	unsigned int  LableEx[32];		//������
	unsigned char Lable[32];		//������
	float		  inputEmiss;		//���������
	float		  inputReflect;		//���뷴���¶�
	float		  inputDis;			//�������
	float         inputOffset;      //�¶�����
	float		  Area;				//�������
	unsigned char pal;				//��ɫ��
	unsigned char reserved2;
	unsigned char reserved3;
	unsigned char reserved4;
	T_COLOR	 color;						//������ɫ
	T_ALARM  sAlarm;
}STAT_GLOBAL;

//���Լ����¶�
typedef struct stat_point
{
	stat_point()
	{
		inputEmiss = 0.98;
		inputReflect = 20.0;
		inputDis = 2.0;
		inputOffset = 0;
	}
	T_POINT sPoint;
	STAT_TEMPER sTemp;
	unsigned int  LableEx[32];		//������
	unsigned char Lable[32];		//������
	float		  inputEmiss;		//���������
	float		  inputReflect;		//���뷴���¶�
	float		  inputDis;			//�������
	float         inputOffset;      //�¶�����
	float		  Area;				//�������
	unsigned char reserved1;
	unsigned char reserved2;
	unsigned char reserved3;
	unsigned char reserved4;
	T_COLOR	 color;						//������ɫ
	T_ALARM  sAlarm;
}STAT_POINT;

//���Լ������¶�ͳ��
typedef struct stat_line
{
	stat_line()
	{
		inputEmiss = 0.98;
		inputReflect = 20.0;
		inputDis = 2.0;
		inputOffset = 0;
	}
	T_LINE sLine;
	STAT_TEMPER sTemp;
	unsigned int  LableEx[32];		//������
	unsigned char Lable[32];		//������
	float		  inputEmiss;		//���������
	float		  inputReflect;		//���뷴���¶�
	float		  inputDis;			//�������
	float         inputOffset;      //�¶�����
	float		  Area;				//�������
	unsigned char reserved1;
	unsigned char reserved2;
	unsigned char reserved3;
	unsigned char reserved4;
	T_COLOR	 color;					//������ɫ	
	T_ALARM  sAlarm;
}STAT_LINE;

//Բ����Բ)�Լ�Բ���¶�ͳ��
typedef struct stat_circle
{
	stat_circle()
	{
		inputEmiss = 0.98;
		inputReflect = 20.0;
		inputDis = 2.0;
		inputOffset = 0;
	}
	T_CIRCLE sCircle;
	STAT_TEMPER sTemp;
	unsigned int  LableEx[32];		//������
	unsigned char Lable[32];		//������
	float		  inputEmiss;		//���������
	float		  inputReflect;		//���뷴���¶�
	float		  inputDis;			//�������
	float         inputOffset;      //�¶�����
	float		  Area;				//�������
	unsigned char reserved1;
	unsigned char reserved2;
	unsigned char reserved3;
	unsigned char reserved4;
	T_COLOR		 color;					//������ɫ	
	T_ALARM  sAlarm;
}STAT_CIRCLE;

//�����Լ��������¶�ͳ��
typedef struct stat_rect
{
	stat_rect()
	{
		inputEmiss = 0.98;
		inputReflect = 20.0;
		inputDis = 2.0;
		inputOffset = 0;
	}
	T_RECT sRect;
	STAT_TEMPER sTemp;
	unsigned int  LableEx[32];		//������
	unsigned char Lable[32];		//������
	float		  inputEmiss;		//���������
	float		  inputReflect;		//���뷴���¶�
	float		  inputDis;			//�������
	float         inputOffset;      //�¶�����
	float		  Area;				//�������
	unsigned char reserved1;
	unsigned char reserved2;
	unsigned char reserved3;
	unsigned char reserved4;
	T_COLOR	 color;					//������ɫ	
	T_ALARM  sAlarm;
}STAT_RECT;

//���������Լ���������¶�ͳ��
typedef struct stat_polygon
{
	stat_polygon()
	{
		inputEmiss = 0.98;
		inputReflect = 20.0;
		inputDis = 2.0;
		inputOffset = 0;
	}
	T_POLYGON sPolygon;
	STAT_TEMPER sTemp;

	unsigned int  LableEx[32];		//������
	unsigned char Lable[32];		//������
	float		  inputEmiss;		//���������
	float		  inputReflect;		//���뷴���¶�
	float		  inputDis;			//�������
	float         inputOffset;      //�¶�����
	float		  Area;				//�������
	unsigned char reserved1;
	unsigned char reserved2;
	unsigned char reserved3;
	unsigned char reserved4;
	T_COLOR	 color;
	T_ALARM  sAlarm;
}STAT_POLYGON;

//�״�ͼ�¶�ͳ��
typedef struct stat_radar
{
	stat_radar()
	{
		inputEmiss = 0.98;
		inputReflect = 20.0;
		inputDis = 2.0;
		inputOffset = 0;
	}
	T_RADAR sRadar;
	STAT_TEMPER sTemp[64];

	unsigned int  LableEx[32];		//������
	unsigned char Lable[32];		//������
	float		  inputEmiss;		//���������
	float		  inputReflect;		//���뷴���¶�
	float		  inputDis;			//�������
	float         inputOffset;      //�¶�����
	float		  Area;				//�������
	unsigned char reserved1;
	unsigned char reserved2;
	unsigned char reserved3;
	unsigned char reserved4;
	T_COLOR	 color;
	T_ALARM  sAlarm;
}STAT_RADAR;

//���ж��󣬸ýṹ������������в��¶���num��ʾ�����Ͷ���ĸ�������Ӧ��ָ�룬��Ҫָ����¶��󣬸ÿռ���Ҫ��ʹ��ǰ����
typedef struct stat_obj
{
	unsigned char numPt;
	unsigned char numLine;
	unsigned char numCircle;
	unsigned char numRect;
	unsigned char numPolygon;
	unsigned char numRadar;		//���һ��
	unsigned char Reserved2;
	unsigned char Reserved3;

	STAT_GLOBAL sGlobal;
	STAT_POINT	sPt[OBJ_MAX];
	STAT_LINE	sLine[OBJ_MAX];
	STAT_CIRCLE sCircle[OBJ_MAX];
	STAT_RECT	sRect[OBJ_MAX];
	STAT_POLYGON sPolygon[OBJ_MAX];
	STAT_RADAR sRadar[1];
}STAT_OBJ;


//128byte���洢ԭʼ��Ƶʱ����ͷ��Ϣ�ᱣ�浽��Ƶ�ļ��У���ȡʱ���������ͷ��Ϣ�Թ�ʹ��
typedef struct tagSAVEHead
{
	unsigned char  Head[32];
	unsigned short width;
	unsigned short height;
	unsigned int   totalFrames;			//¼����֡��������ʱΪ1
	unsigned short Freq;				//֡Ƶ
	unsigned char  Lens;				//����
	unsigned char  version;				//�����汾
	unsigned int   timelen;				//¼��ʱ��
	unsigned char  timestamp[32];		//¼��/����ʱ��
	unsigned short measuredis;			//¼��/���վ���
	unsigned char  devicetype[16];		//�ͺ�
	unsigned char  serialnum[24];		//���к�
	unsigned char  Reserved1;
	unsigned char  Reserved2;

	unsigned short BackgroundTemp; //ҽ�ƴ�ͼʱ�ı����¶�,  t*10
	signed   char  BackgroundCorrTemp; //ҽ�ƴ�ͼʱ�ı���У���¶�,  t*10
	unsigned char  Reserved3;
}T_SAVE_HEAD;

typedef struct tagTIME {
	unsigned short year;
	unsigned short month;
	unsigned short day;
	unsigned short hour;
	unsigned short minute;
	unsigned short sencond;
	unsigned short millisecond;
} T_TIME;


//512byte���豸��Ϣ�������ַ�����ʽ�洢���ÿռ��Ѿ����٣�����Ҫ������ռ�
typedef struct tagDeviceID
{
	unsigned char  Name[32];			//�豸����
	unsigned char  Model[32];		//�豸�ͺ�
	unsigned char  SerialNum[32];	//�豸���к�
	unsigned char  Lens[32];			//��ͷ���
	unsigned char  FactoryTime[32];	//����ʱ��
	unsigned char  WorkTime[32];		//����ʱ��
	unsigned char  Mac[32];			//MAC��ַ
	unsigned char  IP[32];			//IP��ַ
	unsigned char  TempRange[32];   //�¶ȷ�Χ
	unsigned char  Reserved2[32];
	unsigned char  Reserved3[32];
	unsigned char  Reserved4[32];
	unsigned char  Reserved5[32];
	unsigned char  Reserved6[32];
	unsigned char  Reserved7[32];
	unsigned char  Reserved8[32];
}T_DEVICE_INFO;

typedef struct tagIPADDR
{
	char IPAddr[32];			//IP
	unsigned char Reserved[32]; //����
	unsigned int DataPort;		//Port
	unsigned char isValid;		//�Ƿ�����
	unsigned char totalOnline;  //���߸���
	unsigned char Index;        //���б��е�����
}T_IPADDR;

//������������
enum T_PARAMTYPE
{
	paramDevice			= 0,			//�豸����
	paramDownSample		= 1,			//������
	paramReserved0		= 2,			//δ��
	paramFPS		    = 3,			//��ȡ֡Ƶ
	paramReserved1		= 4,			//δ��
	paramSpaceFilter	= 5,			//�����˲�
	paramEnhanceImage	= 6,			//��ǿ
	paramTempSegSel		= 7,			//�¶ȶ�ѡ��
	paramEmit   		= 8,			//��ȡ������õķ�����
	paramDistance		= 9,			//��ȡ������õľ���
	paramReflect		= 10,			//��ȡ������õķ����¶�
	paramCaliSwitch		= 11,			//����У������
	paramReserved5		= 12,           //����
	paramObjTempFilterNum	= 13,		//ʱ���˲�֡��
	paramEnableExtInqure	= 14,       //�Ƿ���Ҫ��ѯ�ⲿ�´��¶ȣ���֧���������´����豸��
	paramSetExtBlackTemp	= 15,       //�����ⲿ�����¶ȣ���֧��û�������´��������ú�����豸��
	paramAutoSelTempSelSw	= 16,		//�Զ�ѡ���¶ȶο���
	paramObjTempFilterSw    = 17,		//���¶���ʱ������˲�����
	paramImageFlip			= 18,		//ͼ����
	paramFocusStatus		= 19,		//�Զ��۽�״̬
	paramTempLensSel		= 20,		//��ͷѡ��
};


#ifndef _T_CTRLPROTOCOL			//��ֹ�ظ�����
#define _T_CTRLPROTOCOL
//�˶���������
enum T_PARAMCTRL
{
	//Э��ѡ��
	paramPelcod 		= 0,		   //pelco-d
	paramUserDef1		= 1,		   //�Զ���Э��1�������ˣ�
	paramUserDef2		= 2,		   //�Զ���Э��2�������
	paramUserDef3		= 3,		   //�Զ���Э��3��ת̨��
	paramUserDef4		= 21,		   //�Զ���Э��4��������ģʽ��

	//����
	paramCtrlUp			= 4,			//��
	paramCtrlDown		= 5,			//��
	paramCtrlLeft		= 6,			//��
	paramCtrlRight		= 7,			//��
	paramCtrlStop		= 8,			//ֹͣ
	paramCtrlBaudRate	= 9,			//������
	paramCtrlLeftUp		= 10,			//����
	paramCtrlLeftDown   = 11,			//����
	paramCtrlRightUp	= 12,			//����
	paramCtrlRightDown  = 13,			//����
	paramCtrlPreSet		= 14,			//����Ԥ��λ
	paramCtrlPreCall	= 15,			//����Ԥ��λ
	paramCtrlPreClear	= 16,			//���Ԥ��λ

	paramMtGetUDPos		= 51,			//��ȡλ��
	paramMtGetLRPos		= 52,			//��ȡλ��
	paramMtSetUDPos		= 53,			//���ó�ʼλ��
	paramMtSetLRPos		= 54,			//���ó�ʼλ��

};
#endif

/*============================================================================
function:	IRSDK_GetVersion:��ȡsdk�汾��Ϣ,��ʽ��v2.2.3.0
parameter:	char *version�����հ汾���ַ���ָ�룬����ǰ���ٿռ䣬����16Byte
return:		int:-1���쳣
0������
history:	null
==============================================================================*/
IR_SDK_API int IRSDK_GetVersion(char *version);

/*============================================================================
function:	IRSDK_Init:��ʼ��sdk,ʵ���Ǵ����㲥�˿ڣ����ڽ��չ㲥��ip��Ϣ
parameter:	void
return:		int: 1����ʼ�����
history:	null
==============================================================================*/
IR_SDK_API int IRSDK_Init(void);

/*============================================================================
function:	IRSDK_Quit:�˳�sdk,�ͷ����һ�����
parameter:	void
return:		int: -1��δ��ʼ�������һ�����������
				0�� �ͷųɹ�
history:	null
==============================================================================*/
IR_SDK_API int IRSDK_Quit(void);

/*============================================================================
function:	IRSDK_SetIPAddrArray:����IP��ַ�Ľ��յ�ַ��ָ��������SDKά��
parameter:	void * pIpInfo������һ��T_IPADDR���飬�������ָ�봫�룬����������sdkά��,���ý��յ�ַ֮����Ҫһ��ʱ������豸����Ҫ��������IRSDK_Create���������
return:		int: 0
history:	null
==============================================================================*/
IR_SDK_API int IRSDK_SetIPAddrArray(void * pIpInfo);

/*============================================================================
function:	IRSDK_Create:����������
parameter:	int handle�����ֵ����ͬ�ĺ������ͬ��ֵ��0~31)
			T_IPADDR sIPAddr: ��ѯ���ĺ���IP��ַ��Ϣ
			CBF_IR cbf_stm�����ݶ˿ڻص�����
			CBF_IR cbf_cmd������˿ڻص�������һ������ΪNULL
			CBF_IR cbf_comm��ͨ�Ŷ˿ڻص�������һ������ΪNULL
			void * param���ص���������
return:		int: 0�� �����ɹ�
				-1����Ч���
history:	null
==============================================================================*/
IR_SDK_API int IRSDK_Create(int handle, T_IPADDR sIPAddr, CBF_IR cbf_stm, CBF_IR cbf_cmd, CBF_IR cbf_comm, void * param = 0);

/*============================================================================
function:	IRSDK_Connect:���Ӻ������
parameter:	int handle��������
return:		int: 0������ͳɹ�������ͳɹ�������ʾ�������������
				-1����Ч���
history:	null
==============================================================================*/
IR_SDK_API int IRSDK_Connect(int handle);

/*============================================================================
function:	IRSDK_Destroy:ֹͣ���ӣ����پ��
parameter:	int handle��������
return:		int: 0�����ٳɹ�
				-1����Ч���
history:	null
==============================================================================*/
IR_SDK_API int IRSDK_Destroy(int handle);

/*============================================================================
function:	IRSDK_Play:���Ͳ�������
parameter:	int handle��������
return:		int: 0������ͳɹ�
				-1����Ч���
history:	null
==============================================================================*/
IR_SDK_API int IRSDK_Play(int handle);

/*============================================================================
function:	IRSDK_Stop:����ֹͣ���ֹͣ�����ݻص�����������
parameter:	int handle��������
return:		int: 0������ͳɹ�
				-1����Ч���
history:	null
==============================================================================*/
IR_SDK_API int IRSDK_Stop(int handle);

/*============================================================================
function:	IRSDK_SetIP:����ֹͣ���ֹͣ�����ݻص�����������
parameter:	int handle��������
return:		int: 0������ͳɹ�
				-1����Ч���
history:	null
==============================================================================*/
IR_SDK_API int IRSDK_SetIP(int handle, char * pIp);

/*============================================================================
function:	IRSDK_Command:����˿ڷ������ݣ�����+����
parameter:	int handle��������
			int cmd������
			int param������
return:		int: 0������ͳɹ�
				-1����Ч���
history:	null
==============================================================================*/
IR_SDK_API int IRSDK_Command(int handle, int command, int param);

/*============================================================================
function:	IRSDK_Calibration:����У������
parameter:	int handle��������
return:		int: 0������ͳɹ�
				-1����Ч���
history:	null
==============================================================================*/
IR_SDK_API int IRSDK_Calibration(int handle);

/*============================================================================
function:	IRSDK_NearFarFocus:�綯��ͷ����
parameter:	int handle��������
			unsigned int param�����Ʒ�ʽ, ��ͷ�ļ�����
return:		int: 0������ͳɹ�
				-1���������
				������ΪFOCUSSTATUS,����ֵΪ�۽�״̬��0��ʾ�Զ��۽���ɣ�1��ʾ���ڽ����Զ��۽�
history:	null
==============================================================================*/
IR_SDK_API int IRSDK_NearFarFocus(int handle, unsigned int param);

/*============================================================================
function:	IRSDK_IsConnected:��ѯ����Ƿ�����������ʹ�ô˺���ʱ��IRSDK_InquireIP����Ҫ��ʱִ�У�
parameter:	int handle��������
return:		int: 0��������������������쳣���ص�������
				1����������������ص�������
history:	null
==============================================================================*/
IR_SDK_API int IRSDK_IsConnected(int handle);

/*============================================================================
function:	IRSDK_InquireDeviceInfo:��ѯ�豸��Ϣ
parameter:	int handle��������
			T_DEVICE_INFO* pDevInfo������ָ�룬������Ϣ�����ú���ǰ���ٿռ䣩
return:		int: 0������ͳɹ�
				-1������������ָ��Ϊ��
history:	null
==============================================================================*/
IR_SDK_API int IRSDK_InquireDeviceInfo(int handle, T_DEVICE_INFO* pDevInfo);

/*============================================================================
function:	IRSDK_ParamCfg:���ò���
parameter:	int handle��������
			T_PARAMTYPE mParamType���������ͣ���ͷ�ļ�
			float f32Param:����ֵ
return:		int: 0������ͳɹ�
				-1���������
history:	null
==============================================================================*/
IR_SDK_API int IRSDK_ParamCfg(int handle, T_PARAMTYPE mParamType, float f32Param);

/*============================================================================
function:	IRSDK_Frame2Gray:�Զ����⣨ԭʼ����ת�Ҷ����ݣ�
parameter:	Frame *pFrame��֡�ṹָ��
			unsigned short *pGray: �ҶȽ����0~255����������ushort
			float f32Constrast:�Աȶ�
			float f32Bright������
			unsigned short u16TFilterCoef��ʱ���˲���0~100��0�ǹرգ�100ʱ���˲�Ч��������1ʱЧ����ã�
return:		int:0
history:	null
==============================================================================*/
IR_SDK_API int IRSDK_Frame2Gray(Frame *pFrame, unsigned short *pGray, float f32Constrast, float f32Bright, unsigned short u16TFilterCoef);

/*============================================================================
function:	IRSDK_Frame2GrayDDE:��DDE���ܵ��Զ����⣨ԭʼ����ת�Ҷ����ݣ�
parameter:	Frame *pFrame��֡�ṹָ��
unsigned short *pGray: �ҶȽ����0~255����������ushort
float f32Constrast:�Աȶ�
float f32Bright������
unsigned short u16TFilterCoef��ʱ���˲���0~100��0�ǹرգ�100ʱ���˲�Ч��������1ʱЧ����ã�
unsigned char u8DDEcoef��DDEϵ����0~100��0�ǹرգ�
unsigned char u8Gamma��gammaУ����0~10��0�ǹرգ�
return:		int:0
history:	null
==============================================================================*/
IR_SDK_API int IRSDK_Frame2GrayDDE(Frame *pFrame, unsigned short *pGray, float f32Constrast, float f32Bright, unsigned short u16TFilterCoef, unsigned char u8DDEcoef, unsigned char u8Gamma);

/*============================================================================
function:	IRSDK_FrameAgc:������⣨ԭʼ����ת�Ҷ����ݣ�
parameter:	Frame *pFrame��֡�ṹָ��
			unsigned short *pGray: �ҶȽ����0~255����Ϊ�˼��ݣ�������ushort
			float f32Constrast:�Աȶ�
			float f32Bright������
			float f32MinT : ���ⷶΧ�������ֵ��
			float f32MaxT : ���ⷶΧ�������ֵ����f32MinT��f32MaxT���ʱ���ú���Ϊ�Զ����⣩��
			STAT_RECT *pRect�������ṩ�ľ�����������������趨���ⷶΧ����ʹ�����ʱ��f32MinT��f32MaxT ������Ч������Ҫʹ��ʱ����ΪNULL
			unsigned char u8Method�����ⷽ��������Ϊ0��
			unsigned short u16TFilterCoef��ʱ���˲���0~100��0�ǹرգ�100ʱ���˲�Ч��������1ʱЧ����ã�
			unsigned char u8DDEcoef��DDEϵ����0~100��0�ǹرգ�
			unsigned char u8Gamma��gammaУ����0~10��0�ǹرգ�
return:		int:0
history:	null
==============================================================================*/
IR_SDK_API int IRSDK_FrameAgc(Frame *pFrame, unsigned short *pGray, float f32Constrast, float f32Bright, float f32MinT, float f32MaxT, STAT_RECT *pRect, unsigned char u8Method, unsigned short u16TFilterCoef);

/*============================================================================
function:	IRSDK_FrameAgcDDE:��DDE���ܵ�������⣨ԭʼ����ת�Ҷ����ݣ�
parameter:	Frame *pFrame��֡�ṹָ��
			unsigned short *pGray: �ҶȽ����0~255����Ϊ�˼��ݣ�������ushort
			float f32Constrast:�Աȶ�
			float f32Bright������
			float f32MinT : ���ⷶΧ�������ֵ��
			float f32MaxT : ���ⷶΧ�������ֵ����f32MinT��f32MaxT���ʱ���ú���Ϊ�Զ����⣩��
			STAT_RECT *pRect�������ṩ�ľ�����������������趨���ⷶΧ����ʹ�����ʱ��f32MinT��f32MaxT ������Ч������Ҫʹ��ʱ����ΪNULL
			unsigned short u16TFilterCoef��ʱ���˲���0~100��0�ǹرգ�100ʱ���˲�Ч��������1ʱЧ����ã�
			unsigned char u8DDEcoef��DDEϵ����0~100��0�ǹرգ�
			unsigned char u8Gamma��gammaУ����0~10��0�ǹرգ�
return:		int:0
history:	null
==============================================================================*/
IR_SDK_API int IRSDK_FrameAgcDDE(Frame *pFrame, unsigned short *pGray, float f32Constrast, float f32Bright, float f32MinT, float f32MaxT, STAT_RECT *pRect, unsigned char u8Method, unsigned short u16TFilterCoef, unsigned char u8DDECoef, unsigned char u8Gamma);

/*============================================================================
function:	IRSDK_Gray2Rgb:�Ҷ�ͼ��תrgb
parameter:	unsigned short* pGray������Ҷ�ָ�룬����ǰ���ٿռ䣬�����С(54+4*256)Byte
			unsigned char* pRgb�����rgb���ڵ���ǰ���ٿռ䣬��ʽΪbgr888
			unsigned short Width:ͼ����
			unsigned short Height:ͼ��߶�
			int PalType����ɫ�����ͣ�0��ҵ1ҽ�ƣ�
			int Pal: ��ɫ����������ҵʱ0~14��ҽ��ʱ0~6��
return:		int:0
history:	null
==============================================================================*/
IR_SDK_API int IRSDK_Gray2Rgb(unsigned short* pGray, unsigned char* pRgb, unsigned short Width, unsigned short Height, int PalType, int Pal);

/*============================================================================
function:	IRSDK_GetPaletteJpeg:��ȡ��ɫ��jpgͼ��
parameter:	unsigned char* pPaletteJpeg�����jpegͼ�񣬵���ǰ���ٿռ䣬�����С��256*3)Byte
			unsigned int *pJpegLen�������Ӧ����Ч����
			unsigned char Method:����Ϊ0
			int PalType����ɫ�����ͣ�0��ҵ1ҽ�ƣ�
			int Pal: ��ɫ����������ҵʱ0~14��ҽ��ʱ0~6��
return:		int:0
history:	null
==============================================================================*/
IR_SDK_API int IRSDK_GetPaletteJpeg(unsigned char* pPaletteJpeg, unsigned int *pJpegLen, unsigned char Method, int PalType, int Pal);

/*============================================================================
function:	IRSDK_GetPaletteBmp:��ȡ��ɫ��bmpͼ��
parameter:	unsigned char* pPaletteBmp�����jpegͼ�񣬵���ǰ���ٿռ䣬�����С(54+4*256)Byte
			unsigned int *pBmpLen�������Ӧ����Ч����
			unsigned char Method:����Ϊ0
			int PalType����ɫ�����ͣ�0��ҵ1ҽ�ƣ�
			int Pal: ��ɫ����������ҵʱ0~14��ҽ��ʱ0~6��
return:		int:0
history:	null
==============================================================================*/
IR_SDK_API int IRSDK_GetPaletteBmp(unsigned char* pPaletteBmp, unsigned int *pBmpLen, unsigned char Method, int PalType, int Pal);

/*============================================================================
function:	IRSDK_GetAnyPointTemp:��ȡ�������¶ȣ������ڻ�ȡ��괦���¶�
parameter:	Frame *pFrame������֡�ṹָ��
STAT_POINT *pPointStat���������Ϣ
return:		int:0
history:	null
==============================================================================*/
IR_SDK_API int IRSDK_GetAnyPointTemp(Frame *pFrame, STAT_POINT *pPointStat);

/*============================================================================
function:	IRSDK_GetGlobalTemp:��ȡȫ���������¶�
parameter:	Frame *pFrame������֡�ṹָ��
			STAT_GLOBAL* pGlobalStat������ȫ����Ϣ
return:		int:-1��ָ���쳣
			0������
history:	null
==============================================================================*/
IR_SDK_API int IRSDK_GetGlobalTemp(Frame *pFrame, STAT_GLOBAL* pGlobalStat);

/*============================================================================
function:	IRSDK_GetPointTemp:��ȡ���¶�
parameter:	Frame *pFrame������֡�ṹָ��
			STAT_POINT *pPointStat���������Ϣ
			unsigned char statIndex:��������ʱ���˲�����ʱ���˲������ã�ʱ���˲�һ�㲻��Ҫ��
return:		int:-1��ָ���쳣
                0������
history:	null
==============================================================================*/
IR_SDK_API int IRSDK_GetPointTemp(Frame *pFrame, STAT_POINT *pPointStat, unsigned char index);

/*============================================================================
function:	IRSDK_GetLineTemp:��ȡ���¶�
parameter:	Frame *pFrame������֡�ṹָ��
			STAT_LINE *pLineStat����������Ϣ
			unsigned char statIndex:��������ʱ���˲�����ʱ���˲������ã�ʱ���˲�һ�㲻��Ҫ��
return:		int:-1��ָ���쳣
				0������
history:	null
==============================================================================*/
IR_SDK_API int IRSDK_GetLineTemp(Frame *pFrame, STAT_LINE *pLineStat, unsigned char index);

/*============================================================================
function:	IRSDK_GetCircleTemp:��ȡԲ���¶�
parameter:	Frame *pFrame������֡�ṹָ��
			STAT_CIRCLE *pCircleStat������Բ��Ϣ
			unsigned char statIndex:��������ʱ���˲�����ʱ���˲������ã�ʱ���˲�һ�㲻��Ҫ��
return:		int:-1��ָ���쳣
				0������
history:	null
==============================================================================*/
IR_SDK_API int IRSDK_GetCircleTemp(Frame *pFrame, STAT_CIRCLE *pCircleStat, unsigned char index);

/*============================================================================
function:	IRSDK_GetRectTemp:��ȡ�����¶�
parameter:	Frame *pFrame������֡�ṹָ��
			STAT_RECT *pRectStat�����������Ϣ
			unsigned char statIndex:��������ʱ���˲�����ʱ���˲������ã�ʱ���˲�һ�㲻��Ҫ��
return:		int:-1��ָ���쳣
				0������
history:	null
==============================================================================*/
IR_SDK_API int IRSDK_GetRectTemp(Frame *pFrame, STAT_RECT *pRectStat, unsigned char index);

/*============================================================================
function:	IRSDK_GetPolygonTemp:��ȡ������¶�
parameter:	Frame *pFrame������֡�ṹָ��
			STAT_POLYGON *pPolygonStat������������Ϣ
			unsigned char statIndex:��������ʱ���˲�����ʱ���˲������ã�ʱ���˲�һ�㲻��Ҫ��
return:		int:-1��ָ���쳣
				0������
history:	null
==============================================================================*/
IR_SDK_API int IRSDK_GetPolygonTemp(Frame *pFrame, STAT_POLYGON *pPolygonStat, unsigned char index);

/*============================================================================
function:	IRSDK_GetPointTemp:��ȡ��ɫ��jpgͼ��
parameter:	Frame *pFrame������֡�ṹָ��
			STAT_OBJ *pObjStat��������¶�����Ϣ
return:		int:-1��ָ���쳣
			0������
history:	null
==============================================================================*/
IR_SDK_API int IRSDK_GetObjTemp(Frame *pFrame, STAT_OBJ *pObjStat);

/*============================================================================
function:	IRSDK_Rgb2Bmp:rgbתbmpͼ��
parameter:	unsigned char * pBmpData�����ת�����Ӧ��BMPͼ�񻺴棬����ǰ���٣���С��(54+w * h * 3) Byte
			unsigned int *pLen�����bmp��Ч����
			unsigned char* pRgb������rgbָ��
			unsigned short Width:ͼ����
			unsigned short Height:ͼ��߶�
return:		int:0
history:	null
==============================================================================*/
IR_SDK_API int IRSDK_Rgb2Bmp(unsigned char * pBmpData, unsigned int *pLen, unsigned char* pRgb, unsigned short Width, unsigned short Height);

/*============================================================================
function:	IRSDK_Rgb2Jpeg:rgbתjpgͼ��
parameter:	unsigned char * pJpegout�����ת�����Ӧ��JPGͼ�񻺴棬����ǰ���٣���С��(w*h*3)Byte
			unsigned int *pLen�����bmp��Ч����
			int quality��ѹ������������80���ϣ�10~100��
			unsigned char* pRgb������rgbָ��
			unsigned short Width:ͼ����
			unsigned short Height:ͼ��߶�
return:		int:0
history:	null
==============================================================================*/
IR_SDK_API int IRSDK_Rgb2Jpeg(unsigned char * pJpegout, unsigned int *pLen, int quality, unsigned char * pRgb, unsigned short Width, unsigned short Height);

/*============================================================================
function:	IRSDK_SaveFrame2Jpeg:�洢rgb���¶�Ϊjpegͼ��
parameter:	char* pFile���ļ�·�����ļ��������ܳ���512byte
			Frame *pFrame��֡�ṹ
			unsigned char *pRgb��rgb����
			unsigned char isSaveObj���Ƿ�洢obj
			STAT_OBJ *pObj��objָ��
return:		int:-2��ָ���쳣
				-1���ļ������쳣
				0������
history:	null
==============================================================================*/
IR_SDK_API int IRSDK_SaveFrame2Jpeg(char* pFile, Frame *pFrame, unsigned char* pRgb, unsigned char isSaveObj, STAT_OBJ *pObj);

/*============================================================================
function:	IRSDK_ReadJpeg2Frame: ����Jpegͼ�񣬻�ȡ֡�Ͳ��¶���
parameter:	char *pFile��������ļ���
			Frame *pFrame������֡
			unsigned char isLoadObj���Ƿ���ز��¶���
			STAT_OBJ *pObj��������¶���ָ��
return:		int:-2��ָ���쳣
				-1���ļ������쳣
				0������
history:	null
==============================================================================*/
IR_SDK_API int IRSDK_ReadJpeg2Frame(char *pFile, Frame *pFrame, unsigned char isLoadObj, STAT_OBJ *pObj);

/*============================================================================
function:	IRSDK_SaveFrame2Video: ����gcv��Ƶ
parameter:	char *pFile��������ļ���
			Frame *pFrame������֡
			unsigned char Op������
			unsigned char isSaveObj���Ƿ񱣴���¶���
			STAT_OBJ *pObj��������¶���
			unsigned char * pThreadBuf������ռ䣬����ǰ���ٵĿռ䣬��Ҫ1024byte�������ļ��ڼ��ַ���ֲ���
return:		int:-2��ָ���쳣
				-1���ļ��쳣
				0������
history:	null
==============================================================================*/
IR_SDK_API int IRSDK_SaveFrame2Video(char *pFile, Frame *pFrame, unsigned char Op, unsigned char isSaveObj, STAT_OBJ *pObj, unsigned char * pThreadBuf);

/*============================================================================
function:	IRSDK_ReadVideo2Frame: ����gcv��Ƶ
parameter:	char *pFile��������ļ���
			Frame *pFrame������֡
			unsigned int Index��֡���������Դ��м俪ʼ
			unsigned char Op����������
			T_SAVE_HEAD *pVideoHead��������ͷ��Ϣ
			STAT_OBJ *pObj��������¶���
			unsigned char * pThreadBuf������ռ䣬����ǰ���ٵĿռ䣬��Ҫ1024byte�����ļ��ڼ��ַ���ֲ���
return :	int:-2���ļ��򿪴���
				-1����ʽ����
				0������
history:	null
==============================================================================*/
IR_SDK_API int IRSDK_ReadVideo2Frame(char *pFile, Frame *pFrame, unsigned int Index, unsigned char Op, T_SAVE_HEAD *pVideoHead, STAT_OBJ *pObj, unsigned char * pThreadBuf);

/*============================================================================
function:	IRSDK_SaveRgb2AVI:rgbͼתaviͼ��
parameter:	char* pFile���ļ�·�����ļ��������ܳ���512byte
			unsigned char *pRgb��rgbָ��
			unsigned char Op������
			int PalType����ɫ�����ͣ�0��ҵ1ҽ�ƣ�
			int Pal: ��ɫ����������ҵʱ0~14��ҽ��ʱ0~6��
			unsigned char * pThreadBuf������ռ䣬����ǰ���ٵĿռ䣬��Ҫ1024byte�������ļ��ڼ��ַ���ֲ���
return:		int:0
history:	null
==============================================================================*/
IR_SDK_API int IRSDK_SaveRgb2AVI(char* pFile, unsigned char *pRgb, unsigned short Width, unsigned short Height, unsigned char Op, int quality, unsigned char * pThreadBuf);

/*============================================================================
function:	IRSDK_SaveObj2CSV: �洢�¶�����
parameter:	char *pFile��������ļ���
			unsigned char Op����������
			STAT_OBJ *pObj��������¶���
			STAT_TEMPER *pGlobalTemper��ȫ���¶�
			unsigned char * pThreadBuf�����ڶ��߳̿��ٵĿռ䣬����1024byte�������ļ��ڼ��ַ���ֲ���
return:		-2��ָ���쳣
			-1���ļ��쳣
			0������
history:	null
==============================================================================*/
IR_SDK_API int IRSDK_SaveObj2CSV(char *pFile, unsigned char Op, STAT_OBJ *pObj, unsigned char * pThreadBuf);

/*============================================================================
function:	IRSDK_SaveFrame2CSV: ����֡�¶���ϢΪcsv
parameter:	char *pFile��������ļ���
			Frame *pFrame������֡�ṹ
			float emit�������ʣ�Ĭ��0.98
			float dis�����룬Ĭ��2
			float reflect�������¶ȣ�Ĭ��20.0
return:		-2��ָ���쳣
			-1���ļ��쳣
			0������
history:	null
==============================================================================*/
IR_SDK_API int IRSDK_SaveFrame2CSV(char* pFile, Frame *pFrame, float emit, float dis, float reflect, float tempoffset);

/*============================================================================
function:	IRSDK_SaveMultiFrame2CSV: ����������֡�¶���ϢΪcsv
parameter:	char *pFile��������ļ���
			Frame *pFrame������֡�ṹ
			float emit�������ʣ�Ĭ��0.98
			float dis�����룬Ĭ��2
			float reflect�������¶ȣ�Ĭ��20.0
return:		-2��ָ���쳣
			-1���ļ��쳣
			0������
history:	null
==============================================================================*/
IR_SDK_API int IRSDK_SaveMultiFrame2CSV(char* pFile, unsigned char Op, Frame *pFrame, float emit, float dis, float reflect, float tempoffset, unsigned char * pThreadBuf);

/*============================================================================
function:	IRSDK_SaveLine2CSV: �������¶���ϢΪcsv
parameter:	char *pFile��������ļ���
			Frame *pFrame������֡�ṹ
			STAT_LINE sLine������Ϣ
return:		-2��ָ���쳣
			-1���ļ��쳣
			0������
history:	null
==============================================================================*/
IR_SDK_API int IRSDK_SaveLine2CSV(char* pFile, Frame *pFrame, STAT_LINE sLine, unsigned char u8Format);

/*============================================================================
function:	IRSDK_SaveRect2CSV: ��������¶���ϢΪcsv
parameter:	char *pFile��������ļ���
			Frame *pFrame������֡�ṹ
			STAT_RECT sRect��������Ϣ
return:		-2��ָ���쳣
			-1���ļ��쳣
			0������
history:	null
==============================================================================*/
IR_SDK_API int IRSDK_SaveRect2CSV(char* pFile, Frame *pFrame, STAT_RECT sRect);

/*============================================================================
function:	IRSDK_SaveCircle2CSV: ����Բ�¶���ϢΪcsv
parameter:	char *pFile��������ļ���
			Frame *pFrame������֡�ṹ
			STAT_CIRCLE sCircle��Բ��Ϣ
return:		-2��ָ���쳣
			-1���ļ��쳣
			0������
history:	null
==============================================================================*/
IR_SDK_API int IRSDK_SaveCircle2CSV(char* pFile, Frame *pFrame, STAT_CIRCLE sCircle);

/*============================================================================
function:	IRSDK_SavePolygon2CSV: �������¶���ϢΪcsv
parameter:	char *pFile��������ļ���
			Frame *pFrame������֡�ṹ
			STAT_POLYGON sPolygon���������Ϣ
return:		-2��ָ���쳣
			-1���ļ��쳣
			0������
history:	null
==============================================================================*/
IR_SDK_API int IRSDK_SavePolygon2CSV(char* pFile, Frame *pFrame, STAT_POLYGON sPolygon);

/*============================================================================
function:	IRSDK_TempPntCorrect:��ĳһ�¶�ֵ����У�����ṩ��������һ��У����ʽ
parameter:	float f32Emiss��������(0.01~1.00)
			float f32Reflect�������¶�
			float f32Dis:����
			float offset: �¶�ƫ��У��
			float temp����У���¶�ֵ
return:		float: У������¶�ֵ
history:	null
==============================================================================*/
IR_SDK_API float IRSDK_TempPntCorrect(float f32Emiss, float f32Reflect, float f32Dis, float offset, float temp);


/*============================================================================
function:	IRSDK_MoveCtrl: �˶����ƣ���ҪӲ��֧��
parameter:	int handle�����
T_PARAMCTRL mProtocol��Э��ѡ��
T_PARAMCTRL mType�����Ʒ�ʽ
unsigned int u32Param�����Ʋ���
return:		-1������쳣
0������
history:	null
==============================================================================*/
IR_SDK_API int IRSDK_MoveCtrl(int handle, T_PARAMCTRL mProtocol, T_PARAMCTRL mType, unsigned int u32Param);


#endif
