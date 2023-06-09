Option Strict Off
Option Explicit On
Module AXC
    '****************************************************************************
    '*****************************************************************************
    '**
    '** File Name
    '** -----------
    '**
    '** AXC.vb
    '**
    '** COPYRIGHT (c) AJINEXTEK Co., LTD
    '**
    '*****************************************************************************
    '*****************************************************************************
    '**
    '** Description
    '** -----------
    '** Ajinextek Counter Library Header File
    '** 
    '**
    '*****************************************************************************
    '*****************************************************************************
    '**
    '** Source Change Indices
    '** ---------------------
    '** 
    '** (None)
    '**
    '**
    '*****************************************************************************
    '*****************************************************************************
    '**
    '** Website
    '** ---------------------
    '**
    '** http://www.ajinextek.com
    '**
    '*****************************************************************************
    '*****************************************************************************
    '*
    '//========== 보드 및 모듈 정보 
    '// CNT 모듈이 있는지 확인
    Public Declare Function AxcInfoIsCNTModule Lib "AXL.dll" (ByRef upStatus As Integer) As Integer

    '// CNT 모듈 No 확인
    Public Declare Function AxcInfoGetModuleNo Lib "AXL.dll" (ByVal lBoardNo As Integer, ByVal lModulePos As Integer, ByRef lpModuleNo As Integer) As Integer

    '// CNT 입출력 모듈의 개수 확인
    Public Declare Function AxcInfoGetModuleCount Lib "AXL.dll" (ByRef lpModuleCount As Integer) As Integer

    '// 지정한 모듈의 카운터 입력 채널 개수 확인
    Public Declare Function AxcInfoGetChannelCount Lib "AXL.dll" (ByVal lModuleNo As Integer, ByRef lpCount As Integer) As Integer

    '// 시스템에 장착된 카운터의 전 채널 수를 확인
    Public Declare Function AxcInfoGetTotalChannelCount Lib "AXL.dll" (ByRef lpChannelCount As Integer) As Integer

    '// 지정한 모듈 번호로 베이스 보드 번호, 모듈 위치, 모듈 ID 확인
    Public Declare Function AxcInfoGetModule Lib "AXL.dll" (ByVal lModuleNo As Integer, ByRef lpBoardNo As Integer, ByRef lpModulePos As Integer, ByRef upModuleID As Integer) As Integer

    '// 해당 모듈이 제어가 가능한 상태인지 반환한다.
    Public Declare Function AxcInfoGetModuleStatus Lib "AXL.dll" (ByVal lModuleNo As Integer) As Integer

    Public Declare Function AxcInfoGetFirstChannelNoOfModuleNo Lib "AXL.dll" (ByVal lModuleNo As Integer, ByRef lpChannelNo As Integer) As Integer

    '// 카운터 모듈의 Encoder 입력 방식을 설정 합니다.
    '// dwMethod --> 0x00 : Sign and pulse, x1 multiplication
    '// dwMethod --> 0x01 : Phase-A and phase-B pulses, x1 multiplication
    '// dwMethod --> 0x02 : Phase-A and phase-B pulses, x2 multiplication
    '// dwMethod --> 0x03 : Phase-A and phase-B pulses, x4 multiplication
    '// dwMethod --> 0x08 : Sign and pulse, x2 multiplication
    '// dwMethod --> 0x09 : Increment and decrement pulses, x1 multiplication
    '// dwMethod --> 0x0A : Increment and decrement pulses, x2 multiplication
    '// SIO-CN2CH의 경우
    '// dwMethod --> 0x00 : Up/Down 방식, A phase : 펄스, B phase : 방향
    '// dwMethod --> 0x01 : Phase-A and phase-B pulses, x1 multiplication
    '// dwMethod --> 0x02 : Phase-A and phase-B pulses, x2 multiplication
    '// dwMethod --> 0x03 : Phase-A and phase-B pulses, x4 multiplication
    Public Declare Function AxcSignalSetEncInputMethod Lib "AXL.dll" (ByVal lChannelNo As Integer, ByVal dwMethod As Integer) As Integer

    '// 카운터 모듈의 Encoder 입력 방식을 설정 합니다.
    '// *dwpUpMethod --> 0x00 : Sign and pulse, x1 multiplication
    '// *dwpUpMethod --> 0x01 : Phase-A and phase-B pulses, x1 multiplication
    '// *dwpUpMethod --> 0x02 : Phase-A and phase-B pulses, x2 multiplication
    '// *dwpUpMethod --> 0x03 : Phase-A and phase-B pulses, x4 multiplication
    '// *dwpUpMethod --> 0x08 : Sign and pulse, x2 multiplication
    '// *dwpUpMethod --> 0x09 : Increment and decrement pulses, x1 multiplication
    '// *dwpUpMethod --> 0x0A : Increment and decrement pulses, x2 multiplication
    '// SIO-CN2CH의 경우
    '// dwMethod --> 0x00 : Up/Down 방식, A phase : 펄스, B phase : 방향
    '// dwMethod --> 0x01 : Phase-A and phase-B pulses, x1 multiplication
    '// dwMethod --> 0x02 : Phase-A and phase-B pulses, x2 multiplication
    '// dwMethod --> 0x03 : Phase-A and phase-B pulses, x4 multiplication
    Public Declare Function AxcSignalGetEncInputMethod Lib "AXL.dll" (ByVal lChannelNo As Integer, ByRef dwpUpMethod As Integer) As Integer

    '// 카운터 모듈의 트리거를 설정 합니다.
    '// dwMode -->  0x00 : Latch
    '// dwMode -->  0x01 : State
    '// dwMode -->  0x02 : Special State    --> SIO-CN2CH 전용
    '// SIO-CN2CH의 경우
    '// dwMode -->  0x00 : 절대 위치 트리거 또는 주기 위치 트리거.
    '// 주의 : 제품마다 기능이 서로 다르기 때문에 구별하여 사용 필요.
    '// dwMode -->  0x01 : 시간 주기 트리거(AxcTriggerSetFreq로 설정)
    Public Declare Function AxcTriggerSetFunction Lib "AXL.dll" (ByVal lChannelNo As Integer, ByVal dwMode As Integer) As Integer

    '// 카운터 모듈의 트리거 설정을 확인 합니다.
    '// *dwMode -->  0x00 : Latch
    '// *dwMode -->  0x01 : State
    '// *dwMode -->  0x02 : Special State
    '// dwMode -->  0x00 : 절대 위치 트리거 또는 주기 위치 트리거.
    '// 주의 : 제품마다 기능이 서로 다르기 때문에 구별하여 사용 필요.
    '// dwMode -->  0x01 : 시간 주기 트리거(AxcTriggerSetFreq로 설정)
    Public Declare Function AxcTriggerGetFunction Lib "AXL.dll" (ByVal lChannelNo As Integer, ByRef dwpMode As Integer) As Integer

    '// dwUsage --> 0x00 : Trigger Not use
    '// dwUsage --> 0x01 : Trigger use
    Public Declare Function AxcTriggerSetNotchEnable Lib "AXL.dll" (ByVal lChannelNo As Integer, ByVal dwUsage As Integer) As Integer

    '// *dwUsage --> 0x00 : Trigger Not use
    '// *dwUsage --> 0x01 : Trigger use
    Public Declare Function AxcTriggerGetNotchEnable Lib "AXL.dll" (ByVal lChannelNo As Integer, ByRef dwpUsage As Integer) As Integer


    '// 카운터 모듈의 Captuer 극성을 설정 합니다.(External latch input polarity)
    '// dwCapturePol --> 0x00 : Signal OFF -> ON
    '// dwCapturePol --> 0x01 : Signal ON -> OFF
    Public Declare Function AxcSignalSetCaptureFunction Lib "AXL.dll" (ByVal lChannelNo As Integer, ByVal dwCapturePol As Integer) As Integer

    '// 카운터 모듈의 Captuer 극성 설정을 확인 합니다.(External latch input polarity)
    '// *dwCapturePol --> 0x00 : Signal OFF -> ON
    '// *dwCapturePol --> 0x01 : Signal ON -> OFF
    Public Declare Function AxcSignalGetCaptureFunction Lib "AXL.dll" (ByVal lChannelNo As Integer, ByRef dwpCapturePol As Integer) As Integer

    '// 카운터 모듈의 Captuer 위치를 확인 합니다.(External latch)
    '// *dbpCapturePos --> Capture position 위치
    Public Declare Function AxcSignalGetCapturePos Lib "AXL.dll" (ByVal lChannelNo As Integer, ByRef dbpCapturePos As Double) As Integer

    '// 카운터 모듈의 카운터 값을 확인 합니다.
    '// *dbpActPos --> 카운터 값
    Public Declare Function AxcStatusGetActPos Lib "AXL.dll" (ByVal lChannelNo As Integer, ByRef dbpActPos As Double) As Integer

    '// 카운터 모듈의 카운터 값을 dbActPos 값으로 설정 합니다.
    Public Declare Function AxcStatusSetActPos Lib "AXL.dll" (ByVal lChannelNo As Integer, ByVal dbActPos As Double) As Integer

    '// 카운터 모듈의 트리거 위치를 설정합니다.
    '// 카운터 모듈의 트리거 위치는 2개까지만 설정 할 수 있습니다.
    Public Declare Function AxcTriggerSetNotchPos Lib "AXL.dll" (ByVal lChannelNo As Integer, ByVal dbLowerPos As Double, ByVal dbUpperPos As Double) As Integer

    '// 카운터 모듈의 설정한 트리거 위치를 확인 합니다.
    Public Declare Function AxcTriggerGetNotchPos Lib "AXL.dll" (ByVal lChannelNo As Integer, ByRef dbpLowerPos As Double, ByRef dbpUpperPos As Double) As Integer

    '// 카운터 모듈의 트리거 출력을 강제로 합니다.
    '// dwOutVal --> 0x00 : 트리거 출력 '0'
    '// dwOutVal --> 0x01 : 트리거 출력 '1'
    Public Declare Function AxcTriggerSetOutput Lib "AXL.dll" (ByVal lChannelNo As Integer, ByVal dwOutVal As Integer) As Integer

    '// 카운터 모듈의 상태를 확인합니다.
    '// Bit '0' : Carry (카운터 현재치가 덧셈 펄스에 의해 카운터 상한치를 넘어 0로 바뀌었을 때 1스캔만 ON으로 합니다.)
    '// Bit '1' : Borrow (카운터 현재치가 뺄셈 펄스에 의해 0을 넘어 카운터 상한치로 바뀌었을 때 1스캔만 ON으로 합니다.)
    '// Bit '2' : Trigger output status
    '// Bit '3' : Latch input status
    Public Declare Function AxcStatusGetChannel Lib "AXL.dll" (ByVal lChannelNo As Integer, ByRef dwpChannelStatus As Integer) As Integer


    '// SIO-CN2CH 전용 함수군
    '//
    '// 카운터 모듈의 위치 단위를 설정한다.
    '// 실제 위치 이동량에 대한 펄스 갯수를 설정하는데,
    '// ex) 1mm 이동에 1000가 필요하다면dMoveUnitPerPulse = 0.001로 입력하고,
    '//     이후 모든 함수의 위치와 관련된 값을 mm 단위로 설정하면 된다.
    Public Declare Function AxcMotSetMoveUnitPerPulse Lib "AXL.dll" (ByVal lChannelNo As Integer, ByVal dMoveUnitPerPulse As Double) As Integer

    '// 카운터 모듈의 위치 단위를 확인한다.
    Public Declare Function AxcMotGetMoveUnitPerPulse Lib "AXL.dll" (ByVal lChannelNo As Integer, ByRef dpMoveUnitPerPuls As Double) As Integer

    '// 카운터 모듈의 엔코더 입력 카운터를 반전 기능을 설정한다.
    '// dwReverse --> 0x00 : 반전하지 않음.
    '// dwReverse --> 0x01 : 반전.
    Public Declare Function AxcSignalSetEncReverse Lib "AXL.dll" (ByVal lChannelNo As Integer, ByVal dwReverse As Integer) As Integer

    '// 카운터 모듈의 엔코더 입력 카운터를 반전 기능을 설정을 확인한다.
    Public Declare Function AxcSignalGetEncReverse Lib "AXL.dll" (ByVal lChannelNo As Integer, ByRef dwpReverse As Integer) As Integer

    '// 카운터 모듈의 Encoder 입력 신호를 선택한다.
    '// dwSource -->  0x00 : 2(A/B)-Phase 신호.
    '// dwSource -->  0x01 : Z-Phase 신호.(방향성 없음.)
    Public Declare Function AxcSignalSetEncSource Lib "AXL.dll" (ByVal lChannelNo As Integer, ByVal dwSource As Integer) As Integer

    '// 카운터 모듈의 Encoder 입력 신호 선택 설정을 확인한다.
    Public Declare Function AxcSignalGetEncSource Lib "AXL.dll" (ByVal lChannelNo As Integer, ByRef dwpSource As Integer) As Integer

    '// 카운터 모듈의 트리거 출력 범위 중 하한 값을 설정한다.
    '// 위치 주기 트리거 제품의 경우 위치 주기로 트리거 출력을 발생시킬 범위 중 하한 값을 설정한다.
    '// 절대 위치 트리거 제품의 경우 Ram 시작 번지의 트리거 정보의 적용 기준 위치를 설정한다.
    '// 단위 : AxcMotSetMoveUnitPerPulse로 설정한 단위.
    '// Note : 하한값은 상한값보다 작은값을 설정하여야 합니다.
    Public Declare Function AxcTriggerSetBlockLowerPos Lib "AXL.dll" (ByVal lChannelNo As Integer, ByVal dLowerPosition As Double) As Integer

    '// 카운터 모듈의 트리거 출력 범위 중 하한 값을 확인한다.
    Public Declare Function AxcTriggerGetBlockLowerPos Lib "AXL.dll" (ByVal lChannelNo As Integer, ByRef dpLowerPosition As Double) As Integer

    '// 카운터 모듈의 트리거 출력 범위 중 상한 값을 설정한다.
    '// 위치 주기 트리거 제품의 경우 위치 주기로 트리거 출력을 발생시킬 범위 중 상한 값을 설정한다.
    '// 절대 위치 트리거 제품의 경우 트리거 정보가 설정된 Ram 의 마지막 번지의 트리거 정보가 적용되는 위치로 사용된다.
    '// 단위 : AxcMotSetMoveUnitPerPulse로 설정한 단위.
    '// Note : 상한값은 하한값보다 큰값을 설정하여야 합니다.
    Public Declare Function AxcTriggerSetBlockUpperPos Lib "AXL.dll" (ByVal lChannelNo As Integer, ByVal dUpperPosition As Double) As Integer
    '// 카운터 모듈의 트리거 출력 범위 중 하한 값을 설정한다.
    Public Declare Function AxcTriggerGetBlockUpperPos Lib "AXL.dll" (ByVal lChannelNo As Integer, ByRef dpUpperrPosition As Double) As Integer

    '// 카운터 모듈의 위치 주기 모드 트리거에 사용되는 위치 주기를 설정한다.
    '// 단위 : AxcMotSetMoveUnitPerPulse로 설정한 단위.
    Public Declare Function AxcTriggerSetPosPeriod Lib "AXL.dll" (ByVal lChannelNo As Integer, ByVal dPeriod As Double) As Integer

    '// 카운터 모듈의 위치 주기 모드 트리거에 사용되는 위치 주기를 확인한다.
    Public Declare Function AxcTriggerGetPosPeriod Lib "AXL.dll" (ByVal lChannelNo As Integer, ByRef dpPeriod As Double) As Integer

    '// 카운터 모듈의 위치 주기 모드 트리거 사용시 위치 증감에 대한 유효기능을 설정한다.
    '// dwDirection -->  0x00 : 카운터의 증/감에 대하여 트리거 주기 마다 출력.
    '// dwDirection -->  0x01 : 카운터가 증가 할때만 트리거 주기 마다 출력.
    '// dwDirection -->  0x01 : 카운터가 감소 할때만 트리거 주기 마다 출력.
    Public Declare Function AxcTriggerSetDirectionCheck Lib "AXL.dll" (ByVal lChannelNo As Integer, ByVal dwDirection As Integer) As Integer
    '// 카운터 모듈의 위치 주기 모드 트리거 사용시 위치 증감에 대한 유효기능 설정을 확인한다.
    Public Declare Function AxcTriggerGetDirectionCheck Lib "AXL.dll" (ByVal lChannelNo As Integer, ByRef dwpDirection As Integer) As Integer

    '// 카운터 모듈의 위치 주기 모드 트리거 기능의 범위, 위치 주기를 한번에 설정한다.
    '// 위치 설정 단위 :  AxcMotSetMoveUnitPerPulse로 설정한 단위.
    Public Declare Function AxcTriggerSetBlock Lib "AXL.dll" (ByVal lChannelNo As Integer, ByVal dLower As Double, ByVal dUpper As Double, ByVal dABSod As Double) As Integer

    '// 카운터 모듈의 위치 주기 모드 트리거 기능의 범위, 위치 주기를 설정을 한번에 확인한다.
    Public Declare Function AxcTriggerGetBlock Lib "AXL.dll" (ByVal lChannelNo As Integer, ByRef dpLower As Double, ByRef dpUpper As Double, ByRef dpABSod As Double) As Integer

    '// 카운터 모듈의 트리거 출력 펄스 폭을 설정한다.
    '// 단위 : uSec
    Public Declare Function AxcTriggerSetTime Lib "AXL.dll" (ByVal lChannelNo As Integer, ByVal dTrigTime As Double) As Integer

    '// 카운터 모듈의 트리거 출력 펄스 폭 설정을 확인한다.
    Public Declare Function AxcTriggerGetTime Lib "AXL.dll" (ByVal lChannelNo As Integer, ByRef dpTrigTime As Double) As Integer

    '// 카운터 모듈의 트리거 출력 펄스의 출력 레벨을 설정한다.
    '// dwLevel -->  0x00 : 트리거 출력시 'Low' 레벨 출력.
    '// dwLevel -->  0x00 : 트리거 출력시 'High' 레벨 출력.
    Public Declare Function AxcTriggerSetLevel Lib "AXL.dll" (ByVal lChannelNo As Integer, ByVal dwLevel As Integer) As Integer
    '// 카운터 모듈의 트리거 출력 펄스의 출력 레벨 설정을 확인한다.
    Public Declare Function AxcTriggerGetLevel Lib "AXL.dll" (ByVal lChannelNo As Integer, ByRef dwpLevel As Integer) As Integer

    '// 카운터 모듈의 주파수 트리거 출력 기능에 필요한 주파수를 설정한다.
    '// 단위 : Hz, 범위 : 1Hz ~ 500 kHz
    Public Declare Function AxcTriggerSetFreq Lib "AXL.dll" (ByVal lChannelNo As Integer, ByVal dwFreqency As Integer) As Integer
    '// 카운터 모듈의 주파수 트리거 출력 기능에 필요한 주파수를 설정을 확인한다.
    Public Declare Function AxcTriggerGetFreq Lib "AXL.dll" (ByVal lChannelNo As Integer, ByRef dwpFreqency As Integer) As Integer

    '// 카운터 모듈의 지정 채널에 대한 범용 출력 값을 설정한다.
    '// dwOutput 범위 : 0x00 ~ 0x0F, 각 채널당 4개의 범용 출력
    Public Declare Function AxcSignalWriteOutput Lib "AXL.dll" (ByVal lChannelNo As Integer, ByVal dwOutput As Integer) As Integer

    '// 카운터 모듈의 지정 채널에 대한 범용 출력 값을 확인한다.
    Public Declare Function AxcSignalReadOutput Lib "AXL.dll" (ByVal lChannelNo As Integer, ByRef dwpOutput As Integer) As Integer

    '// 카운터 모듈의 지정 채널에 대한 범용 출력 값을 비트 별로 설정한다.
    '// lBitNo 범위 : 0 ~ 3, 각 채널당 4개의 범용 출력
    Public Declare Function AxcSignalWriteOutputBit Lib "AXL.dll" (ByVal lChannelNo As Integer, ByVal lBitNo As Integer, ByVal uOnOff As Integer) As Integer
    '// 카운터 모듈의 지정 채널에 대한 범용 출력 값을 비트 별로 확인 한다.
    '// lBitNo 범위 : 0 ~ 3
    Public Declare Function AxcSignalReadOutputBit Lib "AXL.dll" (ByVal lChannelNo As Integer, ByVal lBitNo As Integer, ByRef upOnOff As Integer) As Integer

    '// 카운터 모듈의 지정 채널에 대한 범용 입력 값을 확인한다.
    Public Declare Function AxcSignalReadInput Lib "AXL.dll" (ByVal lChannelNo As Integer, ByRef dwpInput As Integer) As Integer

    '// 카운터 모듈의 지정 채널에 대한 범용 입력 값을 비트 별로 확인 한다.
    '// lBitNo 범위 : 0 ~ 3
    Public Declare Function AxcSignalReadInputBit Lib "AXL.dll" (ByVal lChannelNo As Integer, ByVal lBitNo As Integer, ByRef upOnOff As Integer) As Integer

    '// 카운터 모듈의 트리거 출력을 활성화 한다.
    '// 현재 설정된 기능에 따라 트리거 출력이 최종적으로 출력할 것인지 설정한다.
    Public Declare Function AxcTriggerSetEnable Lib "AXL.dll" (ByVal lChannelNo As Integer, ByVal dwUsage As Integer) As Integer

    '// 카운터 모듈의 트리거 출력 활설화 설정 내용을 확인하다.
    Public Declare Function AxcTriggerGetEnable Lib "AXL.dll" (ByVal lChannelNo As Integer, ByRef dwpUsage As Integer) As Integer

    '// 카운터 모듈의 절대위치 트리거 기능을 위해 설정된 RAM 내용을 확인한다.
    '// dwAddr 범위 : 0x0000 ~ 0x1FFFF;
    Public Declare Function AxcTriggerReadAbsRamData Lib "AXL.dll" (ByVal lChannelNo As Integer, ByVal dwAddr As Integer, ByRef dwpData As Integer) As Integer

    '// 카운터 모듈의 절대위치 트리거 기능을 위해 필요한 RAM 내용을 설정한다.
    '// dwAddr 범위 : 0x0000 ~ 0x1FFFF;
    Public Declare Function AxcTriggerWriteAbsRamData Lib "AXL.dll" (ByVal lChannelNo As Integer, ByVal dwAddr As Integer, ByVal dwData As Integer) As Integer

	'// 지정 CNT 채널의 절대 위치 트리거 기능을 위한 DWORD형 위치 정보를 설정한다.
	'//----------------------------------------------------------------------------------------------------------------------------------
	'// 1. AXT_SIO_CN2CH인 경우
	'// dwTrigNum	--> 131072(=0x20000) 개까지 설정 가능
	'// dwTrigPos	--> DWORD형 Data 입력 가능
	'// dwDirection	--> 0x0(default) : dwTrigPos[0], dwTrigPos[1] ..., dwTrigPos[dwTrigNum - 1] 순으로 Data를 Write 한다.
	'//					0x1			 : dwTrigPos[dwTrigNum - 1], dwTrigPos[dwTrigNum - 2], ..., dwTrigPos[0] 순으로 Data를 Write 한다.
	'// *참고* 1) dwDirection: Data Write 순서만 다를 뿐 기능상의 차이 없음
	'//		  2) AXC Manual의 AxcTriggerSetAbs - Description을 참고하여 data를 가공 후 사용해야 함
	'//----------------------------------------------------------------------------------------------------------------------------------
	'// 2. AXT_SIO_HPC4인 경우
	'// dwTrigNum	--> 500 개까지 설정 가능
	'// dwTrigPos	--> DWORD형 Data 입력 가능
	'// dwDirection	--> 0x0(default) : 사용되지 않는 변수로, 입력하지 않아도 된다.
	'//----------------------------------------------------------------------------------------------------------------------------------
	'// 3. AXT_SIO_RCNT2RTEX, AXT_SIO_RCNT2MLIII, AXT_SIO_RCNT2SIIIH, AXT_SIO_RCNT2SIIIH_R인 경우
	'// dwTrigNum	--> 0x200(=512) 개까지 설정 가능
	'// dwTrigPos	--> DWORD형 data 입력 가능
	'// dwDirection	--> 0x0(default) : 사용되지 않는 변수로, 입력하지 않아도 된다.
	'//----------------------------------------------------------------------------------------------------------------------------------
    Public Declare Function AxcTriggerSetAbs Lib "AXL.dll" (ByVal lChannelNo As Integer, ByVal dwTrigNum As Integer, ByRef dwTrigPos As Integer, Optional ByVal dwDirection = 0 As Integer) As Integer
	
	'// 지정 CNT 채널의 절대 위치 트리거 기능을 위한 double형 위치 정보를 설정한다.
	'//----------------------------------------------------------------------------------------------------------------------------------
	'// 1. AXT_SIO_CN2CH인 경우
	'// dwTrigNum	--> 4194304(=0x20000*32) 개까지 설정 가능
	'// dTrigPos		--> double형 data 입력 가능
	'// dwDirection	--> 0x0(default) : dTrigPos[0], dTrigPos[1] ..., dTrigPos[dwTrigNum - 1] 순으로 Data를 Write 한다.
	'//					0x1			 : dTrigPos[dwTrigNum - 1], dTrigPos[dwTrigNum - 2], ..., dTrigPos[0] 순으로 Data를 Write 한다.
	'// *참고* 1) dwDirection: Data Write 순서만 다를 뿐 기능상의 차이 없음
	'//----------------------------------------------------------------------------------------------------------------------------------
	'// 2. AXT_SIO_RCNT2RTEX, AXT_SIO_RCNT2MLIII, AXT_SIO_RCNT2SIIIH_R인 경우
	'// dwTrigNum	--> 0x200(=512) 개까지 설정 가능
	'// dTrigPos		--> double형 data 입력 가능
	'// dwDirection	--> 0x0(default) : 사용되지 않는 변수로, 입력하지 않아도 된다.
	'//----------------------------------------------------------------------------------------------------------------------------------	
    Public Declare Function AxcTriggerSetAbsDouble Lib "AXL.dll" (ByVal lChannelNo As Integer, ByVal dwTrigNum As Integer, ByRef dTrigPos As Double, Optional ByVal dwDirection = 0 As Integer) As Integer

End Module