EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "Smart Air Exchanger"
Date "2021-07-15"
Rev "1"
Comp "Capgemini engineering"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L power:+12V #PWR0101
U 1 1 60E87B58
P 2700 1400
F 0 "#PWR0101" H 2700 1250 50  0001 C CNN
F 1 "+12V" H 2715 1573 50  0000 C CNN
F 2 "" H 2700 1400 50  0001 C CNN
F 3 "" H 2700 1400 50  0001 C CNN
	1    2700 1400
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0102
U 1 1 60E88014
P 2250 1900
F 0 "#PWR0102" H 2250 1650 50  0001 C CNN
F 1 "GND" H 2255 1727 50  0000 C CNN
F 2 "" H 2250 1900 50  0001 C CNN
F 3 "" H 2250 1900 50  0001 C CNN
	1    2250 1900
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x04_Male J6
U 1 1 60E88585
P 9000 1800
F 0 "J6" H 9108 2081 50  0000 C CNN
F 1 "Fan_IN" H 9108 1990 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x04_P2.54mm_Vertical" H 9000 1800 50  0001 C CNN
F 3 "~" H 9000 1800 50  0001 C CNN
	1    9000 1800
	-1   0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x04_Male J7
U 1 1 60E88919
P 9800 1800
F 0 "J7" H 9908 2081 50  0000 C CNN
F 1 "Fan_OUT" H 9908 1990 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x04_P2.54mm_Vertical" H 9800 1800 50  0001 C CNN
F 3 "~" H 9800 1800 50  0001 C CNN
	1    9800 1800
	-1   0    0    -1  
$EndComp
$Comp
L Device:Q_NPN_CBE Q1
U 1 1 60E916C3
P 8600 2900
F 0 "Q1" H 8791 2946 50  0000 L CNN
F 1 "BC547" H 8791 2855 50  0000 L CNN
F 2 "Package_TO_SOT_THT:TO-92_HandSolder" H 8800 3000 50  0001 C CNN
F 3 "~" H 8600 2900 50  0001 C CNN
	1    8600 2900
	1    0    0    -1  
$EndComp
$Comp
L Device:Q_NPN_CBE Q2
U 1 1 60E91C00
P 9200 3550
F 0 "Q2" H 9391 3596 50  0000 L CNN
F 1 "BC547" H 9391 3505 50  0000 L CNN
F 2 "Package_TO_SOT_THT:TO-92_HandSolder" H 9400 3650 50  0001 C CNN
F 3 "~" H 9200 3550 50  0001 C CNN
	1    9200 3550
	1    0    0    -1  
$EndComp
$Comp
L Device:R R1
U 1 1 60E9214D
P 8150 2900
F 0 "R1" V 7943 2900 50  0000 C CNN
F 1 "10k" V 8034 2900 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 8080 2900 50  0001 C CNN
F 3 "~" H 8150 2900 50  0001 C CNN
	1    8150 2900
	0    1    1    0   
$EndComp
$Comp
L Device:R R2
U 1 1 60E92AB8
P 8200 3550
F 0 "R2" V 7993 3550 50  0000 C CNN
F 1 "10k" V 8084 3550 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 8130 3550 50  0001 C CNN
F 3 "~" H 8200 3550 50  0001 C CNN
	1    8200 3550
	0    1    1    0   
$EndComp
$Comp
L Connector:Barrel_Jack_Switch J1
U 1 1 60E938F7
P 1800 1700
F 0 "J1" H 1857 2017 50  0000 C CNN
F 1 "PWR" H 1857 1926 50  0000 C CNN
F 2 "Connector_BarrelJack:BarrelJack_Horizontal" H 1850 1660 50  0001 C CNN
F 3 "~" H 1850 1660 50  0001 C CNN
	1    1800 1700
	1    0    0    -1  
$EndComp
Wire Wire Line
	2100 1600 2250 1600
Wire Wire Line
	2100 1800 2250 1800
Wire Wire Line
	2250 1800 2250 1900
$Comp
L Connector:Conn_01x04_Female J2
U 1 1 60E949F1
P 3450 3600
F 0 "J2" H 3342 3885 50  0000 C CNN
F 1 "Humidity_sensor" H 3342 3794 50  0000 C CNN
F 2 "SummerPractice:HTU21D_board" H 3450 3600 50  0001 C CNN
F 3 "~" H 3450 3600 50  0001 C CNN
	1    3450 3600
	-1   0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x03_Male J3
U 1 1 60E95AC6
P 3450 4500
F 0 "J3" H 3558 4781 50  0000 C CNN
F 1 "Temp_sensor_IN" H 3558 4690 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x03_P2.54mm_Vertical" H 3450 4500 50  0001 C CNN
F 3 "~" H 3450 4500 50  0001 C CNN
	1    3450 4500
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x03_Male J4
U 1 1 60E96049
P 3450 5000
F 0 "J4" H 3558 5281 50  0000 C CNN
F 1 "Temp_sensor_OUT" H 3558 5190 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x03_P2.54mm_Vertical" H 3450 5000 50  0001 C CNN
F 3 "~" H 3450 5000 50  0001 C CNN
	1    3450 5000
	1    0    0    -1  
$EndComp
$Comp
L Device:R R4
U 1 1 60EA2E50
P 5200 2200
F 0 "R4" H 5270 2246 50  0000 L CNN
F 1 "1k" H 5270 2155 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 5130 2200 50  0001 C CNN
F 3 "~" H 5200 2200 50  0001 C CNN
	1    5200 2200
	1    0    0    -1  
$EndComp
$Comp
L Device:R R5
U 1 1 60EA3629
P 5400 2200
F 0 "R5" H 5470 2246 50  0000 L CNN
F 1 "1k" H 5470 2155 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 5330 2200 50  0001 C CNN
F 3 "~" H 5400 2200 50  0001 C CNN
	1    5400 2200
	1    0    0    -1  
$EndComp
$Comp
L Device:R R6
U 1 1 60EA3938
P 5600 2200
F 0 "R6" H 5670 2246 50  0000 L CNN
F 1 "1k" H 5670 2155 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 5530 2200 50  0001 C CNN
F 3 "~" H 5600 2200 50  0001 C CNN
	1    5600 2200
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR07
U 1 1 60EA574B
P 5850 3050
F 0 "#PWR07" H 5850 2800 50  0001 C CNN
F 1 "GND" H 5855 2877 50  0000 C CNN
F 2 "" H 5850 3050 50  0001 C CNN
F 3 "" H 5850 3050 50  0001 C CNN
	1    5850 3050
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR04
U 1 1 60EA5CFA
P 3800 5250
F 0 "#PWR04" H 3800 5000 50  0001 C CNN
F 1 "GND" H 3805 5077 50  0000 C CNN
F 2 "" H 3800 5250 50  0001 C CNN
F 3 "" H 3800 5250 50  0001 C CNN
	1    3800 5250
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR010
U 1 1 60EA60DC
P 8500 2050
F 0 "#PWR010" H 8500 1800 50  0001 C CNN
F 1 "GND" H 8505 1877 50  0000 C CNN
F 2 "" H 8500 2050 50  0001 C CNN
F 3 "" H 8500 2050 50  0001 C CNN
	1    8500 2050
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR014
U 1 1 60EA639A
P 9450 2150
F 0 "#PWR014" H 9450 1900 50  0001 C CNN
F 1 "GND" H 9455 1977 50  0000 C CNN
F 2 "" H 9450 2150 50  0001 C CNN
F 3 "" H 9450 2150 50  0001 C CNN
	1    9450 2150
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR012
U 1 1 60EA65FD
P 8700 3150
F 0 "#PWR012" H 8700 2900 50  0001 C CNN
F 1 "GND" H 8705 2977 50  0000 C CNN
F 2 "" H 8700 3150 50  0001 C CNN
F 3 "" H 8700 3150 50  0001 C CNN
	1    8700 3150
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR013
U 1 1 60EA6A7A
P 9300 3800
F 0 "#PWR013" H 9300 3550 50  0001 C CNN
F 1 "GND" H 9305 3627 50  0000 C CNN
F 2 "" H 9300 3800 50  0001 C CNN
F 3 "" H 9300 3800 50  0001 C CNN
	1    9300 3800
	1    0    0    -1  
$EndComp
Wire Wire Line
	9300 3800 9300 3750
Wire Wire Line
	8700 3150 8700 3100
$Comp
L Connector:Conn_01x05_Male J5
U 1 1 60EA8344
P 3450 5950
F 0 "J5" H 3558 6331 50  0000 C CNN
F 1 "RTC" H 3558 6240 50  0000 C CNN
F 2 "SummerPractice:RTC_DS3231_board" H 3450 5950 50  0001 C CNN
F 3 "~" H 3450 5950 50  0001 C CNN
	1    3450 5950
	1    0    0    -1  
$EndComp
$Comp
L Device:LED_RAGB D1
U 1 1 60EB9AC2
P 5400 1700
F 0 "D1" V 5446 1370 50  0000 R CNN
F 1 "LED_RAGB" V 5355 1370 50  0000 R CNN
F 2 "LED_THT:LED_D5.0mm-4_RGB_Wide_Pins" H 5400 1650 50  0001 C CNN
F 3 "~" H 5400 1650 50  0001 C CNN
	1    5400 1700
	0    -1   -1   0   
$EndComp
Wire Wire Line
	3650 3500 3900 3500
$Comp
L power:GND #PWR03
U 1 1 60EBC71A
P 3800 3850
F 0 "#PWR03" H 3800 3600 50  0001 C CNN
F 1 "GND" H 3805 3677 50  0000 C CNN
F 2 "" H 3800 3850 50  0001 C CNN
F 3 "" H 3800 3850 50  0001 C CNN
	1    3800 3850
	1    0    0    -1  
$EndComp
Wire Wire Line
	3650 3600 3800 3600
Wire Wire Line
	3800 3600 3800 3850
$Comp
L power:GND #PWR01
U 1 1 60EC2CAA
P 2700 1900
F 0 "#PWR01" H 2700 1650 50  0001 C CNN
F 1 "GND" H 2705 1727 50  0000 C CNN
F 2 "" H 2700 1900 50  0001 C CNN
F 3 "" H 2700 1900 50  0001 C CNN
	1    2700 1900
	1    0    0    -1  
$EndComp
Wire Wire Line
	5400 1500 5400 1400
Wire Wire Line
	3650 4600 3800 4600
Wire Wire Line
	3800 4600 3800 5100
Wire Wire Line
	3650 5100 3800 5100
Connection ~ 3800 5100
Wire Wire Line
	3800 5100 3800 5250
Wire Wire Line
	3650 4400 3900 4400
Wire Wire Line
	3900 4400 3900 4050
Wire Wire Line
	3650 4900 3900 4900
Wire Wire Line
	3900 4900 3900 4400
Connection ~ 3900 4400
$Comp
L Device:R R3
U 1 1 60EC707B
P 4100 4250
F 0 "R3" H 4170 4296 50  0000 L CNN
F 1 "4k7" H 4170 4205 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 4030 4250 50  0001 C CNN
F 3 "~" H 4100 4250 50  0001 C CNN
	1    4100 4250
	1    0    0    -1  
$EndComp
Wire Wire Line
	3650 4500 4100 4500
Wire Wire Line
	4100 4500 4100 4400
Wire Wire Line
	3650 5000 4100 5000
Wire Wire Line
	4100 5000 4100 4500
Connection ~ 4100 4500
Wire Wire Line
	4100 4100 4100 4050
Wire Wire Line
	4100 4050 3900 4050
Connection ~ 3900 4050
Wire Wire Line
	3900 4050 3900 3500
Wire Wire Line
	4100 5000 5200 5000
Wire Wire Line
	5200 5000 5200 3900
Wire Wire Line
	5200 3900 6050 3900
Connection ~ 4100 5000
Text Label 4150 3700 2    50   ~ 0
SDA
Text Label 4150 3800 2    50   ~ 0
SCL
Wire Wire Line
	5200 3700 5200 3800
Wire Wire Line
	5200 3800 6050 3800
Wire Wire Line
	3650 3700 4650 3700
Wire Wire Line
	4600 3800 4600 3500
Wire Wire Line
	4600 3500 4750 3500
Wire Wire Line
	3650 3800 4600 3800
Wire Wire Line
	5200 2350 5200 2700
Wire Wire Line
	5200 2700 6050 2700
Wire Wire Line
	5400 2350 5400 2800
Wire Wire Line
	5400 2800 6050 2800
Wire Wire Line
	5600 2350 5600 2900
Wire Wire Line
	5600 2900 6050 2900
Wire Wire Line
	5600 1900 5600 2050
Wire Wire Line
	5400 1900 5400 2050
Wire Wire Line
	5200 1900 5200 2050
Wire Wire Line
	6950 2700 7700 2700
Wire Wire Line
	7700 2700 7700 2900
Wire Wire Line
	7700 2900 8000 2900
Wire Wire Line
	8300 2900 8400 2900
Wire Wire Line
	8700 2700 8700 2000
Wire Wire Line
	8700 2000 8800 2000
Wire Wire Line
	8800 1900 7600 1900
Wire Wire Line
	7600 1900 7600 2800
Wire Wire Line
	7600 2800 6950 2800
$Comp
L power:+12V #PWR011
U 1 1 60ED4FD4
P 8650 1400
F 0 "#PWR011" H 8650 1250 50  0001 C CNN
F 1 "+12V" H 8665 1573 50  0000 C CNN
F 2 "" H 8650 1400 50  0001 C CNN
F 3 "" H 8650 1400 50  0001 C CNN
	1    8650 1400
	1    0    0    -1  
$EndComp
Wire Wire Line
	8650 1400 8650 1500
Wire Wire Line
	8650 1800 8800 1800
Wire Wire Line
	8500 1700 8800 1700
Wire Wire Line
	8500 2050 8500 1700
Wire Wire Line
	8650 1500 9500 1500
Wire Wire Line
	9500 1500 9500 1800
Wire Wire Line
	9500 1800 9600 1800
Connection ~ 8650 1500
Wire Wire Line
	8650 1500 8650 1800
Wire Wire Line
	9450 1700 9600 1700
Wire Wire Line
	6950 2900 7600 2900
Wire Wire Line
	7600 2900 7600 3550
Wire Wire Line
	7600 3550 8050 3550
Wire Wire Line
	8350 3550 9000 3550
Wire Wire Line
	9600 1900 9150 1900
Wire Wire Line
	9150 1900 9150 2450
Wire Wire Line
	9150 2450 7800 2450
Wire Wire Line
	7800 2450 7800 3000
Wire Wire Line
	7800 3000 6950 3000
Wire Wire Line
	9300 3350 9300 2000
Wire Wire Line
	9300 2000 9600 2000
Wire Wire Line
	9450 2150 9450 1700
Wire Wire Line
	5850 3050 5850 2600
Wire Wire Line
	5850 2600 6050 2600
$Comp
L power:+12V #PWR09
U 1 1 60EE7724
P 7050 2450
F 0 "#PWR09" H 7050 2300 50  0001 C CNN
F 1 "+12V" H 7065 2623 50  0000 C CNN
F 2 "" H 7050 2450 50  0001 C CNN
F 3 "" H 7050 2450 50  0001 C CNN
	1    7050 2450
	1    0    0    -1  
$EndComp
Wire Wire Line
	6950 2500 7050 2500
Wire Wire Line
	7050 2500 7050 2450
$Comp
L power:+3V3 #PWR06
U 1 1 60EE9916
P 5400 1400
F 0 "#PWR06" H 5400 1250 50  0001 C CNN
F 1 "+3V3" H 5415 1573 50  0000 C CNN
F 2 "" H 5400 1400 50  0001 C CNN
F 3 "" H 5400 1400 50  0001 C CNN
	1    5400 1400
	1    0    0    -1  
$EndComp
$Comp
L power:+3V3 #PWR08
U 1 1 60EE9BEB
P 6000 2350
F 0 "#PWR08" H 6000 2200 50  0001 C CNN
F 1 "+3V3" H 6015 2523 50  0000 C CNN
F 2 "" H 6000 2350 50  0001 C CNN
F 3 "" H 6000 2350 50  0001 C CNN
	1    6000 2350
	1    0    0    -1  
$EndComp
Wire Wire Line
	6000 2350 6000 2500
Wire Wire Line
	6000 2500 6050 2500
$Comp
L power:+3V3 #PWR05
U 1 1 60EEBD4E
P 3900 3300
F 0 "#PWR05" H 3900 3150 50  0001 C CNN
F 1 "+3V3" H 3915 3473 50  0000 C CNN
F 2 "" H 3900 3300 50  0001 C CNN
F 3 "" H 3900 3300 50  0001 C CNN
	1    3900 3300
	1    0    0    -1  
$EndComp
Wire Wire Line
	3900 3300 3900 3500
Connection ~ 3900 3500
$Comp
L Device:C C1
U 1 1 60EEE0F4
P 2700 1700
F 0 "C1" H 2815 1746 50  0000 L CNN
F 1 "100n" H 2815 1655 50  0000 L CNN
F 2 "Capacitor_THT:C_Rect_L7.0mm_W2.0mm_P5.00mm" H 2738 1550 50  0001 C CNN
F 3 "~" H 2700 1700 50  0001 C CNN
	1    2700 1700
	1    0    0    -1  
$EndComp
Wire Wire Line
	2700 1900 2700 1850
Wire Wire Line
	2700 1550 2700 1500
Wire Wire Line
	2250 1500 2700 1500
Wire Wire Line
	2250 1500 2250 1600
Connection ~ 2700 1500
Wire Wire Line
	2700 1500 2700 1400
$Comp
L Device:CP C2
U 1 1 60EF4943
P 3150 1700
F 0 "C2" H 3268 1746 50  0000 L CNN
F 1 "10u/50V" H 3268 1655 50  0000 L CNN
F 2 "Capacitor_THT:CP_Radial_D5.0mm_P2.50mm" H 3188 1550 50  0001 C CNN
F 3 "~" H 3150 1700 50  0001 C CNN
	1    3150 1700
	1    0    0    -1  
$EndComp
Wire Wire Line
	3150 1550 3150 1500
Wire Wire Line
	3150 1500 2700 1500
$Comp
L power:GND #PWR02
U 1 1 60EF6ECC
P 3150 1900
F 0 "#PWR02" H 3150 1650 50  0001 C CNN
F 1 "GND" H 3155 1727 50  0000 C CNN
F 2 "" H 3150 1900 50  0001 C CNN
F 3 "" H 3150 1900 50  0001 C CNN
	1    3150 1900
	1    0    0    -1  
$EndComp
Wire Wire Line
	3150 1900 3150 1850
$Comp
L power:GND #PWR0103
U 1 1 60F32A7C
P 7100 3100
F 0 "#PWR0103" H 7100 2850 50  0001 C CNN
F 1 "GND" H 7105 2927 50  0000 C CNN
F 2 "" H 7100 3100 50  0001 C CNN
F 3 "" H 7100 3100 50  0001 C CNN
	1    7100 3100
	1    0    0    -1  
$EndComp
Wire Wire Line
	7100 3100 7100 2600
Wire Wire Line
	7100 2600 6950 2600
Wire Wire Line
	3650 5750 3900 5750
Wire Wire Line
	3900 5750 3900 4900
Connection ~ 3900 4900
$Comp
L power:GND #PWR0104
U 1 1 60ECA9BB
P 3800 6250
F 0 "#PWR0104" H 3800 6000 50  0001 C CNN
F 1 "GND" H 3805 6077 50  0000 C CNN
F 2 "" H 3800 6250 50  0001 C CNN
F 3 "" H 3800 6250 50  0001 C CNN
	1    3800 6250
	1    0    0    -1  
$EndComp
Wire Wire Line
	3800 6250 3800 6150
Wire Wire Line
	3800 6150 3650 6150
Wire Wire Line
	3650 5850 4650 5850
Wire Wire Line
	4650 5850 4650 3700
Connection ~ 4650 3700
Wire Wire Line
	4650 3700 5200 3700
Wire Wire Line
	3650 5950 4750 5950
Wire Wire Line
	4750 5950 4750 3500
Connection ~ 4750 3500
Wire Wire Line
	4750 3500 6050 3500
NoConn ~ 3650 6050
$Comp
L SmartAirExchanger:ESP32_DEVKIT_V1 U?
U 1 1 60EFFD22
P 6500 3200
F 0 "U?" H 6500 4125 50  0000 C CNN
F 1 "ESP32_DEVKIT_V1" H 6500 4034 50  0000 C CNN
F 2 "SummerPractice:ESP32_DEVKIT_V1" H 6350 3100 50  0001 C CNN
F 3 "" H 6350 3100 50  0001 C CNN
	1    6500 3200
	1    0    0    -1  
$EndComp
Text Label 7200 2800 0    50   ~ 0
RPM_IN
Text Label 7200 3000 0    50   ~ 0
RPM_OUT
Text Label 7200 2700 0    50   ~ 0
PWM_IN
Text Label 7200 2900 0    50   ~ 0
PWM_OUT
Text Label 5750 3800 0    50   ~ 0
SDA
Text Label 5750 3500 0    50   ~ 0
SCL
Text Label 5600 3900 0    50   ~ 0
OneWire
$EndSCHEMATC
