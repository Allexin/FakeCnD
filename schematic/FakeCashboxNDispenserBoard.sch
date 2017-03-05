EESchema Schematic File Version 2
LIBS:power
LIBS:device
LIBS:transistors
LIBS:conn
LIBS:linear
LIBS:regul
LIBS:74xx
LIBS:cmos4000
LIBS:adc-dac
LIBS:memory
LIBS:xilinx
LIBS:microcontrollers
LIBS:dsp
LIBS:microchip
LIBS:analog_switches
LIBS:motorola
LIBS:texas
LIBS:intel
LIBS:audio
LIBS:interface
LIBS:digital-audio
LIBS:philips
LIBS:display
LIBS:cypress
LIBS:siliconi
LIBS:opto
LIBS:atmel
LIBS:contrib
LIBS:valves
LIBS:FakeCashboxNDispenserBoard-cache
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L Arduino_Nano_Header J1
U 1 1 58A5985D
P 4150 4150
F 0 "J1" H 4150 4950 60  0000 C CNN
F 1 "Arduino_Nano_Header" H 4150 3350 60  0000 C CNN
F 2 "w_conn_misc:arduino_nano_header" H 4150 4150 60  0000 C CNN
F 3 "" H 4150 4150 60  0000 C CNN
	1    4150 4150
	1    0    0    -1  
$EndComp
$Comp
L SW_PUSH SW1
U 1 1 58A5991E
P 6050 3450
F 0 "SW1" H 6200 3560 50  0000 C CNN
F 1 "SW_PUSH" H 6050 3370 50  0000 C CNN
F 2 "Buttons_Switches_ThroughHole:SW_PUSH-12mm" H 6050 3450 50  0000 C CNN
F 3 "" H 6050 3450 50  0000 C CNN
	1    6050 3450
	1    0    0    -1  
$EndComp
$Comp
L SW_PUSH SW2
U 1 1 58A59943
P 6050 3750
F 0 "SW2" H 6200 3860 50  0000 C CNN
F 1 "SW_PUSH" H 6050 3670 50  0000 C CNN
F 2 "Buttons_Switches_ThroughHole:SW_PUSH-12mm" H 6050 3750 50  0000 C CNN
F 3 "" H 6050 3750 50  0000 C CNN
	1    6050 3750
	1    0    0    -1  
$EndComp
$Comp
L SW_PUSH SW3
U 1 1 58A59966
P 6050 4000
F 0 "SW3" H 6200 4110 50  0000 C CNN
F 1 "SW_PUSH" H 6050 3920 50  0000 C CNN
F 2 "Buttons_Switches_ThroughHole:SW_PUSH-12mm" H 6050 4000 50  0000 C CNN
F 3 "" H 6050 4000 50  0000 C CNN
	1    6050 4000
	1    0    0    -1  
$EndComp
$Comp
L CONN_01X04 P1
U 1 1 58A59A0A
P 6050 5050
F 0 "P1" H 6050 5300 50  0000 C CNN
F 1 "CONN_01X04" V 6150 5050 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x04_Pitch2.54mm" H 6050 5050 50  0000 C CNN
F 3 "" H 6050 5050 50  0000 C CNN
	1    6050 5050
	1    0    0    -1  
$EndComp
$Comp
L R R2
U 1 1 58A59E48
P 5000 4600
F 0 "R2" V 5080 4600 50  0000 C CNN
F 1 "R2200Ohm" V 5000 4600 50  0000 C CNN
F 2 "Resistors_ThroughHole:R_Axial_DIN0309_L9.0mm_D3.2mm_P12.70mm_Horizontal" V 4930 4600 50  0000 C CNN
F 3 "" H 5000 4600 50  0000 C CNN
	1    5000 4600
	1    0    0    -1  
$EndComp
$Comp
L R R1
U 1 1 58A59E9D
P 4850 4600
F 0 "R1" V 4930 4600 50  0000 C CNN
F 1 "R2200oHm" V 4850 4600 50  0000 C CNN
F 2 "Resistors_ThroughHole:R_Axial_DIN0309_L9.0mm_D3.2mm_P12.70mm_Horizontal" V 4780 4600 50  0000 C CNN
F 3 "" H 4850 4600 50  0000 C CNN
	1    4850 4600
	1    0    0    -1  
$EndComp
$Comp
L R R3
U 1 1 58A5A03E
P 5050 3200
F 0 "R3" V 5130 3200 50  0000 C CNN
F 1 "R3000oHm" V 5050 3200 50  0000 C CNN
F 2 "Resistors_ThroughHole:R_Axial_DIN0309_L9.0mm_D3.2mm_P12.70mm_Horizontal" V 4980 3200 50  0000 C CNN
F 3 "" H 5050 3200 50  0000 C CNN
	1    5050 3200
	0    1    1    0   
$EndComp
Wire Wire Line
	4500 4150 5400 4150
Wire Wire Line
	5400 4150 5400 5200
Wire Wire Line
	5400 5200 5850 5200
Wire Wire Line
	4500 4050 5500 4050
Wire Wire Line
	5500 4050 5500 5100
Wire Wire Line
	5500 5100 5850 5100
Wire Wire Line
	5000 4750 5400 4750
Connection ~ 5400 4750
Wire Wire Line
	5500 4800 4850 4800
Wire Wire Line
	4850 4800 4850 4750
Connection ~ 5500 4800
Wire Wire Line
	4850 4450 5000 4450
Wire Wire Line
	4850 3200 4850 4450
Wire Wire Line
	4500 3750 5550 3750
Wire Wire Line
	5850 5000 5550 5000
Wire Wire Line
	5550 5000 5550 3750
Connection ~ 4850 3750
Wire Wire Line
	4500 3550 5650 3550
Wire Wire Line
	5650 3550 5650 4900
Wire Wire Line
	5650 4900 5850 4900
Wire Wire Line
	4850 3200 4900 3200
Wire Wire Line
	5200 3200 5750 3200
Wire Wire Line
	5750 3200 5750 4000
Connection ~ 5750 3450
Connection ~ 5750 3750
Wire Wire Line
	6350 4000 6350 4250
Wire Wire Line
	6350 4250 4500 4250
Wire Wire Line
	6350 3750 6500 3750
Wire Wire Line
	6400 3750 6400 4350
Wire Wire Line
	6400 4350 4500 4350
Wire Wire Line
	6350 3450 6500 3450
Wire Wire Line
	6450 3450 6450 4400
Wire Wire Line
	6450 4400 4500 4400
Wire Wire Line
	4500 4400 4500 4450
$Comp
L R R4
U 1 1 58A5A27B
P 6650 3450
F 0 "R4" V 6730 3450 50  0000 C CNN
F 1 "R4700oHm" V 6650 3450 50  0000 C CNN
F 2 "Resistors_ThroughHole:R_Axial_DIN0309_L9.0mm_D3.2mm_P12.70mm_Horizontal" V 6580 3450 50  0000 C CNN
F 3 "" H 6650 3450 50  0000 C CNN
	1    6650 3450
	0    1    1    0   
$EndComp
$Comp
L R R5
U 1 1 58A5A2D3
P 6650 3750
F 0 "R5" V 6730 3750 50  0000 C CNN
F 1 "R4700oHm" V 6650 3750 50  0000 C CNN
F 2 "Resistors_ThroughHole:R_Axial_DIN0309_L9.0mm_D3.2mm_P12.70mm_Horizontal" V 6580 3750 50  0000 C CNN
F 3 "" H 6650 3750 50  0000 C CNN
	1    6650 3750
	0    1    1    0   
$EndComp
$Comp
L R R6
U 1 1 58A5A31E
P 6650 4000
F 0 "R6" V 6730 4000 50  0000 C CNN
F 1 "R4700oHm" V 6650 4000 50  0000 C CNN
F 2 "Resistors_ThroughHole:R_Axial_DIN0309_L9.0mm_D3.2mm_P12.70mm_Horizontal" V 6580 4000 50  0000 C CNN
F 3 "" H 6650 4000 50  0000 C CNN
	1    6650 4000
	0    1    1    0   
$EndComp
Connection ~ 6450 3450
Connection ~ 6400 3750
Wire Wire Line
	6350 4000 6500 4000
Wire Wire Line
	6800 3100 6800 4000
Connection ~ 6800 3750
Wire Wire Line
	6800 3100 4550 3100
Wire Wire Line
	4550 3100 4550 3550
Connection ~ 4550 3550
Connection ~ 6800 3450
$EndSCHEMATC
