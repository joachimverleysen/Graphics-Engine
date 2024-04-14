## Gequoteerde functionaliteit

V: Werkend  
-: Deels werkend met gekende problemen (onderaan beschreven)  
X: Niet werkend of niet geïmplementeerd  
(blanco): TODO  


|   | Functionaliteit      | Status |
|---|---------------------------|-------|
| 1 | 2D L-systemen             | V     |
|   | Met haakjes               |       |
|   | Stochastisch              |       |
| 2 | Transformaties            | V     |
|   | Eye-point                 | V     |
|   | Projectie                 | V     |
| 3 | Platonische Lichamen      | V     |
|   | Kegel en cylinder         | V     |
|   | Bol                       | V     |
|   | Torus                     | V     |
|   | 3D L-systemen             |       |
| 4 | Z-buffering (lijnen)      | V     |
| 5 | Triangulatie              | V     |
|   | Z-buffering (driehoeken)  | -     |
| 6 | 3D fractalen              |       |
|   | BuckyBall                 |       |
|   | Mengerspons               |       |
|   | View Frustum              |       |
| 7 | Ambient licht             |       |
|   | Diffuus licht (oneindig)  |       |
|   | Diffuus licht (puntbron)  |       |
|   | Speculair licht           |       |
| 8 | Schaduw                   |       |
|   | Texture mapping           |       |
| 9 | Bollen en cylinders       |       |
|   | UV-coordinaten            |       |
|   | Cube mapping              |       |

Geïmplementeerde vorm van texture mapping: ...

## Gekende problemen
1. **Inaccurate z-buffering**

    Maybe I make a rounding error somewhere? But I can't find it.

    For example:
      - z_buffering025.ini
      - z_buffering073.ini


2. **Wrong image dimensions**

    - z_buffering079.ini - image is smaller than it should be


## Niet-gequoteerde functionaliteit
...

## Extra functionaliteit, niet in de opgaves beschreven
...

