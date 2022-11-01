# Bill of Materials

This file lists part numbers and suppliers for components on this board.
The goal is to find the cheapest compatible sources, so that its easier 
for folks to build the board cost-effectively.

Also, some parts are only needed if you want particular ports to work.
If you don't need those ports, you may be able to skip some parts, and
save a bit of cost.

## Parts required for core operation of the board

| Positions      | Count | Part          |
|----------------|-------|---------------|
| J2             | 1     | pinheader 2x6 |
| J34, J35       | 2     | pinheader 1x4 |
| U1, U4, U6     | 3     | 74LS595       |
| U2, U5, U7     | 3     | 74LS165       |

## Parts for User Port

| Positions              | Count | Part          |
|------------------------|-------|---------------|
| R1, R2, R3, R4, R5, R6 | 6     | 3.3kΩ         |
| U3, U20, U21, U24      | 4     | 74LS125       |
| U8                     | 1     | 74LS595       |


## Parts for Tape Port

| Positions     | Count | Part      |
|---------------|-------|-----------|
| J18           | 1     | BA60BCOWT |


## Parts for Component Video Port

| Positions     | Count | Part          |
|---------------|-------|---------------|
| J3            | 1     | pinheader 2x6 |
| J24           | 1     | DIN-8 90°     |
| R44, R50, R56 | 3     | 510Ω          |
| R45, R51, R57 | 3     | 1kΩ           |
| R46, R52, R58 | 3     | 2kΩ           |
| R47, R53, R59 | 3     | 4kΩ           |

## Parts for C1565 Port

| Positions | Count | Part                 | digikey.com        | reichelt.de | price |
|-----------|-------|----------------------|--------------------|-------------|-------| 
| J1        | 1     | Mini DIN-8 90°       | 2057-MDJ-008-FS-ND | EB-DIOS M08 | ~$1   |
| J4, J5    | 2     | pinheader 2x17 keyed |                    |             |       |
