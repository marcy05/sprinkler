# Sprinkler Documentation

This folder collects all the Documentation related information.


# Power consumption summary

|Component| Voltage | Current |
| -| -| -|
|ESP32 | 2.2 to 3.3 | 5μA(deep sleep), 15mA(Light Sleep Mode) 500mA(WiFi)|
|   |    |    |
|   |    |    |
|   |    |    |
|   |    |    |
|   |    |    |


## ESP32 project suggested battery voltage

3.7V 

Li-ion 18650 cells provide 3.7V nominal voltage and capacities from 1,000 to over 3,000 mAh. Their larger size accommodates high capacity for long runtime. Requires protection circuit.

LiPo packs output about 3.7V per cell (adjustable via cell configurations), with capacities ranging from under 100 mAh to over 5,000 mAh. Lightweight form factor great for portable/wearable devices.


|Battery| Option|	Voltage	|Capacity Range|	Pros|	Cons|
| -| -| -| -| -| -|
|LiPo Pouch Cell|	3.7V (typ.)|	100 mAh – 5,000 mAh|	Lightweight, versatile form factors|	Needs protection circuit|
|18650 Li-ion Cell|	3.7V (typ.)|	1,000 – 3,500 mAh|	High capacity, ideal voltage|	Can be expensive|

The ESP32 development board provides a couple options for connecting a battery power source:

Vin Pin: The Vin pin feeds through the onboard regulator. This allows voltages up to 16V to be stepped down to a steady 3.3V output.


## PMIC

Reference research link

[DigiKey](https://www.digikey.it/it/products/filter/gestione-dell-alimentazione-pmic/caricabatterie/781?s=N4IgjCBcoCxgTFUBjKAzAhgGwM4FMAaEAeygG0QBmANgFZKAGRAXSIAcAXKEAZQ4CcAlgDsA5iAC%2BRMAE4AHDKQhUkTLkIlyIBiGZSQAWkTRlUAQFcNpSBVogiivUQOUlKi1a1gde-XZNsUGDsgZDwtPrUSoIAJtwG3hDsXJAg9iAcAJ5seNwYOKgSEkA)


Possible battery charger IC

[BQ24040DSQR](https://www.digikey.it/it/products/detail/texas-instruments/BQ24040DSQR/2231631?_gl=1*npb17f*_up*MQ..*_gs*MQ..&gclid=Cj0KCQjw7dm-BhCoARIsALFk4v_X6oxV3m-uqd8AIHoQSnFiqoDmCyz1x7foSuYQAdGnPx5mMPouIAoaAibwEALw_wcB&gclsrc=aw.ds)


[BQ24091DGQR](https://www.digikey.it/it/products/detail/texas-instruments/BQ24091DGQR/2353310?_gl=1*kv66nh*_up*MQ..*_gs*MQ..&gclid=Cj0KCQjw7dm-BhCoARIsALFk4v_X6oxV3m-uqd8AIHoQSnFiqoDmCyz1x7foSuYQAdGnPx5mMPouIAoaAibwEALw_wcB&gclsrc=aw.ds)
