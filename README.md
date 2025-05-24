# GEOMETRIAI MODELLEZÉS<br>Bézier Curve Drawer

### 🔎 Projekt leírás
Ez egy interaktív, Python-alapú Bézier-görbe vizualizáló alkalmazás, amely megjelenít egy *n-ed fokú Bézier görbét* a *De Casteljau-algoritmus* használatával. A pontokat egy 1x1 négyzetrácsos területre lehet elhelyezni, mozgatni és törölni. Ezek mellett megtudja jeleníteni a görbéhez tartozó tangest, a görbe segítőket *(ehhez a göbe kettévágását használtam)*, illetve a kontroll pontok közötti szakaszok hosszát.

**Főbb funkciók:**
1. pont lerakása `<bal klikk>`
2. pont törlése `<jobb klikk>`
3. pont mozgatása `drag & drop`
4. görbe kiszámítása és megjelenítése `[checkbox]`
5. görbe segítők kiszámítása és megjelenítése `[checkbox]`
6. tangens pont kiszámítása és megjelenítése `[checkbox]`
7. pontok közötti távolság kiszámítása és megjelenítése `[checkbox]`
8. t paraméter állíthatósága `[slider]`

**Gombok:**
1. pontok törlése `[Új görbe]`
2. görbe mentése `[Mentés]`
3. görbe betöltése `[Betöltés]`
4. kép készítés `[Export PNG]`

### 📥 Telepítés

1. Telepítse a Python 3.x verziót
2. Telepítse a szükséges függőségeket
   
```bash
pip install -r requirements.txt
```

### ▶️ Futtatás

```bash
python bezier-app.py
```
<br><br>

> név: **Nyíri Dániel**<br>
> neptunkód: **AUGHMI**<br>
> email: nyiridaniel3@gmail.com
