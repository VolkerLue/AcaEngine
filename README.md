# AcaEngine
The Acagamics Game Engine Course

##GuiToolkit

Das GuiToolkit stellt 4 Grundlegende Arten von Gui Objekten zur Verfügung: Slider, Checkboxes, Buttons, Textdisplays und Textfelder. 
Alle Gui Objekte brauchen eine Entität, eine Position und eine Skalierung als Parameter.
Wenn man eine Container verwendet werden Positionierung und Skalierung über den Container verwaltet, man muss aber dennoch einen beliebigen Wert übergeben.

Alle Objekte außer Slider haben ein Display welches als String übergeben wird. Beim Textfeld wird der eingegebenen String durch die Nutzereingabe ersetzt.
Weiterhin wird ein vec4 zur Textfarbe übergeben.

Textdisplays brauchen eine Textur als Hintergrundfarbe.

Buttons, Textfelder und Checkboxen brauchen zwei Texturen als Parameter.
Die erste Textur dient als standart Farbe des Buttons und des Textfeldes und als False State der Checkbox.
Die zweite Textur erscheint wenn man über den Button mit der Maus fährt, auf das Textfeld klickt oder drüberfährt und beim True State der Checkbox.

Slider brauchen keine Texturen, sondern eine Anzahl an möglichen Sliderpositionen und eine Startposition, beide als Integer.
Ein Boolean beim Slider richtet den Slider vertikal (true) oder horizontal (false) aus.

Buttons, Checkboxen und Textfelder können eine Funktion bekommen. Bei Textfeldern wird die Funktion durch einen Enter Tastendruck während man auf das Feld geklickt hat ausgelöst.
Bei Buttons und Checkboxen wird die Funktion durch ein anklicken ausgelöst.
Die Funktionen erhalten die gewünschte Entität und das System als Parameter.

Die getLevel Funktion gibt den Aktuellen Sliderstand als Integer zurück.

Um komplexer Funktionen zu schreiben, zum Beispiel eine Buttonfunktion welche vom Zustand einer Checkbox abhängt, kann man entweder über die ID der Checkbox auf den Boolean via System zugreifen
oder man übergibt eine leere Funktion und schreibt eine neue Funktion, welche die gewünschten Entitäten mitbekommt. Die "Hitbox" des Buttons muss mit der Position und Skalierung berechnet werden.


## Usage

```c++


math::Rectangle buttonBox = math::Rectangle(glm::vec2(position.position[0], position.position[1]), glm::vec2(position.position[0] + scale.scale[0], position.position[1] + scale.scale[1]));

glm::vec2 cursorPos = system.cameraOrthogonal.toWorldSpace(input::InputManager::getCursorPos());

if (buttonBox.isIn(cursorPos) == true){
        //example_Function
}

```

bei einfachen Funktionen wird diese berechnung in einer Updatefunktion im Toolkit berechnet.

##Container

Container übernehmen die Positionierung und Skalierung der einzelnen Objekte. Container haben eine Position, eine Skalierung, Reihen und Spalten Zahl, einen Abstandparameter, eine Hintergrundtextur,
zwei Booleans und einen Vector an Entitäten.
Der erste Boolean gibt an ob die Angegebe Textur als Hintergrund gesetzt wird. Die Textur muss in jedem Fall angegeben werden, wird aber nur bei True verwendet.
Der zweite Boolean bietet die möglichkeit die Container zu ziehen.
