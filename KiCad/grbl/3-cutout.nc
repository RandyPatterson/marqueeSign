(G-CODE GENERATED BY FLATCAM v8.994 - www.flatcam.org - Version Date: 2020/11/7)

(Name: MatrixDisplay-B_Cu.gbr_cutout_cnc)
(Type: G-code from Geometry)
(Units: MM)

(Created on Wednesday, 20 April 2022 at 21:15)

(This preprocessor is the default preprocessor used by FlatCAM.)
(It is made to work with MACH3 compatible motion controllers.)

(TOOL DIAMETER: 1.2 mm)
(Feedrate_XY: 75.0 mm/min)
(Feedrate_Z: 60.0 mm/min)
(Feedrate rapids 1500.0 mm/min)

(Z_Cut: -1.8 mm)
(DepthPerCut: 0.5 mm <=>4 passes)
(Z_Move: 2.0 mm)
(Z Start: None mm)
(Z End: 5.0 mm)
(X,Y End: None mm)
(Steps per circle: 64)
(Preprocessor Geometry: Default_no_M6)

(X range:    5.0547 ...   58.2520  mm)
(Y range:    5.9560 ...   70.1112  mm)

(Spindle Speed: 1000.0 RPM)
G21
G90
G94

G01 F75.00

M5
G00 Z15.0000
G00 X0.0000 Y0.0000
T1
(MSG, Change to Tool Dia = 1.2000)
M0
G00 Z15.0000
        
M03 S1000.0
G4 P1.0
G01 F75.00
G00 X32.1534 Y5.9560
G01 F60.00
G01 Z-0.5000
G01 F75.00
G01 X7.1547 Y5.9560
G01 X6.9489 Y5.9661
G01 X6.7450 Y5.9964
G01 X6.5451 Y6.0464
G01 X6.3511 Y6.1159
G01 X6.1648 Y6.2040
G01 X5.9880 Y6.3099
G01 X5.8225 Y6.4327
G01 X5.6698 Y6.5711
G01 X5.5314 Y6.7238
G01 X5.4086 Y6.8893
G01 X5.3027 Y7.0661
G01 X5.2146 Y7.2524
G01 X5.1451 Y7.4464
G01 X5.0951 Y7.6463
G01 X5.0648 Y7.8502
G01 X5.0547 Y8.0560
G01 X5.0547 Y38.5336
G00 X5.0547 Y38.5336
G01 F60.00
G01 Z-1.0000
G01 F75.00
G01 X5.0547 Y8.0560
G01 X5.0648 Y7.8502
G01 X5.0951 Y7.6463
G01 X5.1451 Y7.4464
G01 X5.2146 Y7.2524
G01 X5.3027 Y7.0661
G01 X5.4086 Y6.8893
G01 X5.5314 Y6.7238
G01 X5.6698 Y6.5711
G01 X5.8225 Y6.4327
G01 X5.9880 Y6.3099
G01 X6.1648 Y6.2040
G01 X6.3511 Y6.1159
G01 X6.5451 Y6.0464
G01 X6.7450 Y5.9964
G01 X6.9489 Y5.9661
G01 X7.1547 Y5.9560
G01 X32.1534 Y5.9560
G00 X32.1534 Y5.9560
G01 F60.00
G01 Z-1.5000
G01 F75.00
G01 X7.1547 Y5.9560
G01 X6.9489 Y5.9661
G01 X6.7450 Y5.9964
G01 X6.5451 Y6.0464
G01 X6.3511 Y6.1159
G01 X6.1648 Y6.2040
G01 X5.9880 Y6.3099
G01 X5.8225 Y6.4327
G01 X5.6698 Y6.5711
G01 X5.5314 Y6.7238
G01 X5.4086 Y6.8893
G01 X5.3027 Y7.0661
G01 X5.2146 Y7.2524
G01 X5.1451 Y7.4464
G01 X5.0951 Y7.6463
G01 X5.0648 Y7.8502
G01 X5.0547 Y8.0560
G01 X5.0547 Y38.5336
G00 X5.0547 Y38.5336
G01 F60.00
G01 Z-1.8000
G01 F75.00
G01 X5.0547 Y8.0560
G01 X5.0648 Y7.8502
G01 X5.0951 Y7.6463
G01 X5.1451 Y7.4464
G01 X5.2146 Y7.2524
G01 X5.3027 Y7.0661
G01 X5.4086 Y6.8893
G01 X5.5314 Y6.7238
G01 X5.6698 Y6.5711
G01 X5.8225 Y6.4327
G01 X5.9880 Y6.3099
G01 X6.1648 Y6.2040
G01 X6.3511 Y6.1159
G01 X6.5451 Y6.0464
G01 X6.7450 Y5.9964
G01 X6.9489 Y5.9661
G01 X7.1547 Y5.9560
G01 X32.1534 Y5.9560
G00 Z2.0000
G00 X5.0547 Y40.5336
G01 F60.00
G01 Z-0.5000
G01 F75.00
G01 X5.0547 Y68.0112
G01 X5.0648 Y68.2171
G01 X5.0951 Y68.4209
G01 X5.1451 Y68.6208
G01 X5.2146 Y68.8149
G01 X5.3027 Y69.0012
G01 X5.4086 Y69.1779
G01 X5.5314 Y69.3435
G01 X5.6698 Y69.4962
G01 X5.8225 Y69.6346
G01 X5.9880 Y69.7573
G01 X6.1648 Y69.8633
G01 X6.3511 Y69.9514
G01 X6.5451 Y70.0208
G01 X6.7450 Y70.0709
G01 X6.9489 Y70.1011
G01 X7.1547 Y70.1112
G01 X32.1534 Y70.1112
G00 X32.1534 Y70.1112
G01 F60.00
G01 Z-1.0000
G01 F75.00
G01 X7.1547 Y70.1112
G01 X6.9489 Y70.1011
G01 X6.7450 Y70.0709
G01 X6.5451 Y70.0208
G01 X6.3511 Y69.9514
G01 X6.1648 Y69.8633
G01 X5.9880 Y69.7573
G01 X5.8225 Y69.6346
G01 X5.6698 Y69.4962
G01 X5.5314 Y69.3435
G01 X5.4086 Y69.1779
G01 X5.3027 Y69.0012
G01 X5.2146 Y68.8149
G01 X5.1451 Y68.6208
G01 X5.0951 Y68.4209
G01 X5.0648 Y68.2171
G01 X5.0547 Y68.0112
G01 X5.0547 Y40.5336
G00 X5.0547 Y40.5336
G01 F60.00
G01 Z-1.5000
G01 F75.00
G01 X5.0547 Y68.0112
G01 X5.0648 Y68.2171
G01 X5.0951 Y68.4209
G01 X5.1451 Y68.6208
G01 X5.2146 Y68.8149
G01 X5.3027 Y69.0012
G01 X5.4086 Y69.1779
G01 X5.5314 Y69.3435
G01 X5.6698 Y69.4962
G01 X5.8225 Y69.6346
G01 X5.9880 Y69.7573
G01 X6.1648 Y69.8633
G01 X6.3511 Y69.9514
G01 X6.5451 Y70.0208
G01 X6.7450 Y70.0709
G01 X6.9489 Y70.1011
G01 X7.1547 Y70.1112
G01 X32.1534 Y70.1112
G00 X32.1534 Y70.1112
G01 F60.00
G01 Z-1.8000
G01 F75.00
G01 X7.1547 Y70.1112
G01 X6.9489 Y70.1011
G01 X6.7450 Y70.0709
G01 X6.5451 Y70.0208
G01 X6.3511 Y69.9514
G01 X6.1648 Y69.8633
G01 X5.9880 Y69.7573
G01 X5.8225 Y69.6346
G01 X5.6698 Y69.4962
G01 X5.5314 Y69.3435
G01 X5.4086 Y69.1779
G01 X5.3027 Y69.0012
G01 X5.2146 Y68.8149
G01 X5.1451 Y68.6208
G01 X5.0951 Y68.4209
G01 X5.0648 Y68.2171
G01 X5.0547 Y68.0112
G01 X5.0547 Y40.5336
G00 Z2.0000
G00 X34.1534 Y70.1112
G01 F60.00
G01 Z-0.5000
G01 F75.00
G01 X56.1520 Y70.1112
G01 X56.3578 Y70.1011
G01 X56.5617 Y70.0709
G01 X56.7616 Y70.0208
G01 X56.9556 Y69.9514
G01 X57.1419 Y69.8633
G01 X57.3187 Y69.7573
G01 X57.4842 Y69.6346
G01 X57.6369 Y69.4962
G01 X57.7753 Y69.3435
G01 X57.8981 Y69.1779
G01 X58.0040 Y69.0012
G01 X58.0921 Y68.8149
G01 X58.1616 Y68.6208
G01 X58.2116 Y68.4209
G01 X58.2419 Y68.2171
G01 X58.2520 Y68.0112
G01 X58.2520 Y40.5336
G00 X58.2520 Y40.5336
G01 F60.00
G01 Z-1.0000
G01 F75.00
G01 X58.2520 Y68.0112
G01 X58.2419 Y68.2171
G01 X58.2116 Y68.4209
G01 X58.1616 Y68.6208
G01 X58.0921 Y68.8149
G01 X58.0040 Y69.0012
G01 X57.8981 Y69.1779
G01 X57.7753 Y69.3435
G01 X57.6369 Y69.4962
G01 X57.4842 Y69.6346
G01 X57.3187 Y69.7573
G01 X57.1419 Y69.8633
G01 X56.9556 Y69.9514
G01 X56.7616 Y70.0208
G01 X56.5617 Y70.0709
G01 X56.3578 Y70.1011
G01 X56.1520 Y70.1112
G01 X34.1534 Y70.1112
G00 X34.1534 Y70.1112
G01 F60.00
G01 Z-1.5000
G01 F75.00
G01 X56.1520 Y70.1112
G01 X56.3578 Y70.1011
G01 X56.5617 Y70.0709
G01 X56.7616 Y70.0208
G01 X56.9556 Y69.9514
G01 X57.1419 Y69.8633
G01 X57.3187 Y69.7573
G01 X57.4842 Y69.6346
G01 X57.6369 Y69.4962
G01 X57.7753 Y69.3435
G01 X57.8981 Y69.1779
G01 X58.0040 Y69.0012
G01 X58.0921 Y68.8149
G01 X58.1616 Y68.6208
G01 X58.2116 Y68.4209
G01 X58.2419 Y68.2171
G01 X58.2520 Y68.0112
G01 X58.2520 Y40.5336
G00 X58.2520 Y40.5336
G01 F60.00
G01 Z-1.8000
G01 F75.00
G01 X58.2520 Y68.0112
G01 X58.2419 Y68.2171
G01 X58.2116 Y68.4209
G01 X58.1616 Y68.6208
G01 X58.0921 Y68.8149
G01 X58.0040 Y69.0012
G01 X57.8981 Y69.1779
G01 X57.7753 Y69.3435
G01 X57.6369 Y69.4962
G01 X57.4842 Y69.6346
G01 X57.3187 Y69.7573
G01 X57.1419 Y69.8633
G01 X56.9556 Y69.9514
G01 X56.7616 Y70.0208
G01 X56.5617 Y70.0709
G01 X56.3578 Y70.1011
G01 X56.1520 Y70.1112
G01 X34.1534 Y70.1112
G00 Z2.0000
G00 X58.2520 Y38.5336
G01 F60.00
G01 Z-0.5000
G01 F75.00
G01 X58.2520 Y8.0560
G01 X58.2419 Y7.8502
G01 X58.2116 Y7.6463
G01 X58.1616 Y7.4464
G01 X58.0921 Y7.2524
G01 X58.0040 Y7.0661
G01 X57.8981 Y6.8893
G01 X57.7753 Y6.7238
G01 X57.6369 Y6.5711
G01 X57.4842 Y6.4327
G01 X57.3187 Y6.3099
G01 X57.1419 Y6.2040
G01 X56.9556 Y6.1159
G01 X56.7616 Y6.0464
G01 X56.5617 Y5.9964
G01 X56.3578 Y5.9661
G01 X56.1520 Y5.9560
G01 X34.1534 Y5.9560
G00 X34.1534 Y5.9560
G01 F60.00
G01 Z-1.0000
G01 F75.00
G01 X56.1520 Y5.9560
G01 X56.3578 Y5.9661
G01 X56.5617 Y5.9964
G01 X56.7616 Y6.0464
G01 X56.9556 Y6.1159
G01 X57.1419 Y6.2040
G01 X57.3187 Y6.3099
G01 X57.4842 Y6.4327
G01 X57.6369 Y6.5711
G01 X57.7753 Y6.7238
G01 X57.8981 Y6.8893
G01 X58.0040 Y7.0661
G01 X58.0921 Y7.2524
G01 X58.1616 Y7.4464
G01 X58.2116 Y7.6463
G01 X58.2419 Y7.8502
G01 X58.2520 Y8.0560
G01 X58.2520 Y38.5336
G00 X58.2520 Y38.5336
G01 F60.00
G01 Z-1.5000
G01 F75.00
G01 X58.2520 Y8.0560
G01 X58.2419 Y7.8502
G01 X58.2116 Y7.6463
G01 X58.1616 Y7.4464
G01 X58.0921 Y7.2524
G01 X58.0040 Y7.0661
G01 X57.8981 Y6.8893
G01 X57.7753 Y6.7238
G01 X57.6369 Y6.5711
G01 X57.4842 Y6.4327
G01 X57.3187 Y6.3099
G01 X57.1419 Y6.2040
G01 X56.9556 Y6.1159
G01 X56.7616 Y6.0464
G01 X56.5617 Y5.9964
G01 X56.3578 Y5.9661
G01 X56.1520 Y5.9560
G01 X34.1534 Y5.9560
G00 X34.1534 Y5.9560
G01 F60.00
G01 Z-1.8000
G01 F75.00
G01 X56.1520 Y5.9560
G01 X56.3578 Y5.9661
G01 X56.5617 Y5.9964
G01 X56.7616 Y6.0464
G01 X56.9556 Y6.1159
G01 X57.1419 Y6.2040
G01 X57.3187 Y6.3099
G01 X57.4842 Y6.4327
G01 X57.6369 Y6.5711
G01 X57.7753 Y6.7238
G01 X57.8981 Y6.8893
G01 X58.0040 Y7.0661
G01 X58.0921 Y7.2524
G01 X58.1616 Y7.4464
G01 X58.2116 Y7.6463
G01 X58.2419 Y7.8502
G01 X58.2520 Y8.0560
G01 X58.2520 Y38.5336
G00 Z2.0000
M05
G00 Z2.0000
G00 Z5.00

