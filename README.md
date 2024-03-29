## Gequoteerde functionaliteit

V: Werkend  
-: Deels werkend met gekende problemen (onderaan beschreven)  
X: Niet werkend of niet geïmplementeerd  
(blanco): TODO  


|   | Functionaliteit      | Status |
|---|---------------------------|--------|
| 1 | 2D L-systemen             | V      |
|   | Met haakjes               |        |
|   | Stochastisch              |        |
| 2 | Transformaties            | V      |
|   | Eye-point                 | V      |
|   | Projectie                 | V      |
| 3 | Platonische Lichamen      | V      |
|   | Kegel en cylinder         | V      |
|   | Bol                       | V      |
|   | Torus                     | V      |
|   | 3D L-systemen             | -      |
| 4 | Z-buffering (lijnen)      |        |
| 5 | Triangulatie              |        |
|   | Z-buffering (driehoeken)  |        |
| 6 | 3D fractalen              |        |
|   | BuckyBall                 |        |
|   | Mengerspons               |        |
|   | View Frustum              |        |
| 7 | Ambient licht             |        |
|   | Diffuus licht (oneindig)  |        |
|   | Diffuus licht (puntbron)  |        |
|   | Speculair licht           |        |
| 8 | Schaduw                   |        |
|   | Texture mapping           |        |
| 9 | Bollen en cylinders       |        |
|   | UV-coordinaten            |        |
|   | Cube mapping              |        |

Geïmplementeerde vorm van texture mapping: ...

## Gekende problemen

3D LSystems don't work. I'm not sure if I modify the vectors properly in MyLSystem3D::_str2Points().
Could you check that please?

(ini-file = "wireframes015.ini")

## Niet-gequoteerde functionaliteit
...

## Extra functionaliteit, niet in de opgaves beschreven
...

