# EVE – Neuronales Grundkonzept
**Stand: 17. November 2026**  
**Status: Grundlagen und erste Architekturüberlegungen – kein abgeschlossenes Konzept**

## 1. Grundgedanke

EVE soll ein biologisch inspiriertes, eigenständig lernendes neuronales System werden.

Ziel ist nicht, menschliches Wissen oder menschliches Verhalten nachzuprogrammieren, sondern die Voraussetzungen zu schaffen, unter denen Lernen, Erinnern, Wiedererkennen und möglicherweise komplexere Fähigkeiten entstehen können.

Wir orientieren uns an der Biologie, ohne diese vollständig simulieren zu wollen.

Der neuronale Kern soll ohne LLM und ohne vorgegebene Semantik funktionieren.

## 2. Kleinste Einheit: Das binäre Signal

**Festgelegter Grundsatz:**

Die kleinste Einheit der neuronalen Signalübertragung ist ein binärer Zustand.

- `0` – kein Impuls
- `1` – Impuls

Dies entspricht unmittelbar der binären Arbeitsweise digitaler Rechner.

Ein Impuls besitzt zunächst keinen eigenen Bedeutungsinhalt. Seine Wirkung ergibt sich aus den empfangenden Neuronen, ihren Zuständen und den bestehenden Verbindungen.

Die Information kann außerdem in der zeitlichen Abfolge von Impulsen liegen.

Wichtig ist die Unterscheidung zwischen dem binären Signal und dem internen Zustand eines Neurons. Dieser muss nicht auf ein einzelnes Bit beschränkt sein.

## 3. Asynchrone Signalverarbeitung

**Festgelegter Grundsatz:**

EVE soll sich am asynchronen Verhalten biologischer Nervensysteme orientieren.

- Kein globaler neuronaler Verarbeitungstakt.
- Neuronen reagieren auf eintreffende Ereignisse.
- Signale können zeitlich versetzt eintreffen.
- Jedes Neuron besitzt einen eigenen internen Zustand.
- Ein fehlender Impuls muss nicht als eigenes Ereignis übertragen werden.

Die zugrunde liegende Computerhardware besitzt selbstverständlich weiterhin einen Takt.

Asynchronität bezieht sich auf das Verhalten des neuronalen Systems.

## 4. Das künstliche Neuron

**Bisheriges experimentelles Grundmodell:**

Ein Neuron besitzt mindestens:

- Einen internen Membranzustand.
- Eine Erregungsschwelle.
- Eingehende Verbindungen.
- Die Fähigkeit, auf Impulse zu reagieren.
- Die Fähigkeit, einen Ausgangsimpuls auszulösen.
- Ein zeitabhängiges Abklingverhalten.

### Beispiel

Ein Neuron besitzt die Erregungsschwelle 3.

1. Ein eintreffender Impuls erhöht den internen Zustand.
2. Weitere Impulse können sich zeitlich aufsummieren.
3. Erreicht der Zustand die Schwelle, feuert das Neuron.
4. Es erzeugt einen binären Ausgangsimpuls.
5. Anschließend wird sein Zustand zurückgesetzt beziehungsweise entsprechend dem gewählten Modell verändert.

Dieses Prinzip entspricht einem stark vereinfachten Integrate-and-Fire-Neuron.

Der Schwellenwert 3 ist ausschließlich ein Beispiel, keine allgemeingültige Festlegung.

## 5. Zeitabhängiger Membranzustand

**Bevorzugter Modellansatz:**

Die Wirkung eintreffender Impulse soll nicht unbegrenzt bestehen bleiben.

Wie bei biologischen Neuronen soll sich der Membranzustand mit der Zeit wieder einem Ruhezustand annähern.

Dadurch können zeitlich nahe Impulse gemeinsam eine Reaktion auslösen, während weit auseinanderliegende Impulse möglicherweise nicht ausreichen.

### Ereignisgesteuerte Berechnung

Um Rechenleistung zu sparen, soll der Membranzustand nicht permanent neu berechnet werden müssen.

Stattdessen kann ein Neuron speichern:

- Den zuletzt berechneten Membranzustand.
- Den Zeitpunkt der letzten Aktualisierung.
- Einen Parameter für das Abklingverhalten.

Bei einem neuen Ereignis wird zunächst die vergangene Zeit berücksichtigt und anschließend die Wirkung des eintreffenden Impulses verarbeitet.

Dieses Prinzip wird als Lazy Evaluation bezeichnet.

Vorteile:

- Keine eigenen Threads pro Neuron notwendig.
- Kein kontinuierliches Aktualisieren aller Neuronen erforderlich.
- Potenziell geringer Rechenaufwand bei geringer Aktivität.
- Geeignet für ressourcenbeschränkte Hardware.

Die konkrete Zerfallsfunktion ist noch nicht festgelegt.

Auch die Behandlung zeitabhängiger Ereignisse ohne neue Eingangsimpulse muss noch untersucht werden.

## 6. Neuronale Verbindungen und Gewichte

**Grundsätzliche Festlegung:**

EVE soll keine Erinnerungen als gesonderte Datensätze mit semantischen Kennzeichnungen speichern.

Das neuronale System besteht aus Neuronen, ihren Zuständen, Verbindungen und Gewichten.

Was wir als Erinnerung bezeichnen, soll aus Veränderungen und Aktivitätsmustern dieser Strukturen hervorgehen.

Verbindungen können die Wirkung eines eintreffenden Impulses beeinflussen.

Ihre Stärke und möglicherweise auch ihre Existenz sollen veränderlich sein.

Die konkrete mathematische Darstellung von Gewichten ist noch offen.

## 7. Lernen und Erinnern

**Festgelegter Grundsatz:**

Lernen soll durch Veränderungen des neuronalen Systems entstehen.

Eine Erfahrung kann Verbindungen, Gewichte oder andere veränderliche neuronale Eigenschaften beeinflussen.

Eine spätere ähnliche Situation kann dadurch eine andere Reaktion auslösen.

Wir unterscheiden funktional:

**Speichern:** Eine Erfahrung bewirkt eine anhaltende Veränderung im neuronalen System.

**Abrufen:** Eine spätere Aktivität nutzt diese veränderte Struktur und kann dadurch ein zuvor entstandenes Muster erneut aktivieren.

**Wiedererkennen:** Ähnliche Eingangssituationen können aufgrund früherer Veränderungen zu entsprechenden Reaktionsmustern führen.

Diese Fähigkeiten sollen nicht als fertige externe Funktionen programmiert werden.

Sie sollen aus den neuronalen Mechanismen hervorgehen.

Eine reproduzierbare Reaktion allein ist noch kein Nachweis von Lernen. Es muss gezeigt werden, dass vorherige Erfahrung das spätere Verhalten tatsächlich verändert hat.

## 8. Vergessen und begrenzte Ressourcen

**Festgelegte philosophische Grundsätze:**

Informationen besitzen keine universelle Eigenschaft wie „gut“, „schlecht“ oder „wichtig“.

Ihre mögliche Bedeutung ergibt sich erst aus Zusammenhängen und Konsequenzen.

EVE soll deshalb keinen fest programmierten Mechanismus erhalten, der Erinnerungen anhand einer vorgegebenen semantischen Wichtigkeit bewertet.

Vergessen kann durch Veränderungen neuronaler Strukturen entstehen.

Beispielsweise durch:

- Veränderungen von Gewichten.
- Veränderungen bestehender Verbindungen.
- Abbau von Verbindungen.
- Überlagerung früherer Aktivitätsmuster.

Dabei ist zu unterscheiden:

**Neuronales Vergessen ist nicht automatisch eine Freigabe von Arbeitsspeicher.**

Ein verändertes Gewicht kann ein früheres Muster unzugänglich machen, ohne Speicherplatz freizugeben.

### Lernen aus dem Vergessen

EVE muss nicht erkennen können, welche konkrete Information sie vergessen hat.

Es ist denkbar, dass spätere Erfahrungen ihr Verhalten so verändern, dass sie in vergleichbaren Situationen andere neuronale Strukturen erhält oder verändert.

Ob und wie das möglich ist, muss experimentell untersucht werden.

## 9. Lernen und Meta-Lernen

**Festgelegte Zielrichtung:**

Das Lernen von Informationen und die Veränderung des eigenen Lernverhaltens sollen möglichst aus denselben grundlegenden neuronalen Mechanismen entstehen.

Es soll nicht vorschnell ein separates System eingeführt werden, das vorgibt, wie EVE zu lernen hat.

Dadurch könnte sich langfristig auch verändern, wie EVE mit neuen Erfahrungen umgeht.

Ob die gewählten Mechanismen tatsächlich Meta-Lernen ermöglichen, ist eine offene Forschungsfrage.

## 10. Entstehung neuer Verbindungen und Neuronen

Hier unterscheiden wir zwei Vorgänge.

### Synaptogenese

Die Entstehung neuer Verbindungen zwischen vorhandenen Neuronen.

Biologische Vorbilder sind unter anderem aktivitätsabhängige Veränderungen und die Bildung neuer synaptischer Kontakte.

Mögliche experimentelle Einflussgrößen:

- Zeitliche Zusammenhänge zwischen Impulsen.
- Wiederholte gemeinsame Aktivität.
- Bestehende neuronale Zustände.
- Verfügbare Ressourcen.

Es ist noch nicht festgelegt, welche Mechanismen EVE verwenden soll.

### Neurogenese

Die Entstehung zusätzlicher Neuronen.

Auch diese Möglichkeit soll grundsätzlich untersucht werden.

Neue Neuronen dürfen jedoch nicht mit neuen Erinnerungen gleichgesetzt werden.

Ein Netzwerk kann möglicherweise allein durch Veränderungen seiner vorhandenen Verbindungen lernen.

Wann und warum neue Neuronen entstehen sollten, ist noch offen.

## 11. Initiale neuronale Struktur

**Festgelegte Zielrichtung:**

EVE soll nicht mit einem einzelnen, isolierten Neuron beginnen.

Das initiale Netzwerk soll bereits ausreichend komplex sein, um grundlegende Gedächtnisfunktionen untersuchen zu können.

Eine kleine Anzahl initial verbundener Neuronen ist ausdrücklich zulässig.

Auch eine vorgegebene Grundverschaltung kommt infrage.

Dabei gilt:

**Angeborene Struktur ist nicht gleich angeborenes Wissen.**

Wir können funktionale Voraussetzungen bereitstellen, ohne konkrete Erinnerungen, Bedeutungen oder Erkennungsmuster vorzugeben.

Die notwendige Anzahl von Neuronen und ihre anfängliche Verschaltung sind noch nicht bestimmt.

## 12. BIOS, Stammhirn und lernendes Netzwerk

Die bereits diskutierte funktionale Trennung bleibt bestehen.

### BIOS

Technische Grundlage des Systems.

Mögliche Aufgaben:

- Speicherverwaltung.
- Zeit- und Ereignisverwaltung.
- Ressourcenbegrenzung.
- Technische Ausführung.
- Hardwareabstraktion.

Das BIOS soll nicht entscheiden, welche Erfahrungen inhaltlich wertvoll sind.

### Stammhirn

Angeborene Regulations- und Grundfunktionen.

Die konkreten Aufgaben und Mechanismen müssen noch ausgearbeitet werden.

Insbesondere ist offen, ob und wie angeborene Regulation neuronale Plastizität beeinflussen soll.

### Lernendes neuronales Netzwerk

Hier sollen durch Aktivität und Erfahrung veränderliche neuronale Strukturen entstehen.

Die Trennung ist zunächst funktional. Sie bedeutet nicht zwangsläufig, dass drei vollständig getrennte Programme entstehen müssen.

## 13. Hardwareunabhängigkeit

**Festgelegter Grundsatz:**

EVE soll auf einem Mikrocontroller grundsätzlich dieselben neuronalen Fähigkeiten besitzen wie auf einem leistungsfähigen Rechner.

Unterschiedlich sind ausschließlich die verfügbaren Ressourcen und damit die realisierbare Größe und Geschwindigkeit des Systems.

Daraus folgen Anforderungen:

- Gemeinsamer neuronaler Kern.
- Keine zwingende Betriebssystemabhängigkeit.
- Keine zwingende GPU-Abhängigkeit.
- Kontrollierbarer Speicherverbrauch.
- Skalierbare Anzahl von Neuronen und Verbindungen.
- Identische grundlegende Lernmechanismen.
- Möglichst ressourcenschonende Berechnungen.

Ein Raspberry Pi Pico dient als mögliche spätere Zielplattform.

Aktuell steht kein Pico zur Verfügung. Die ersten Untersuchungen erfolgen deshalb auf einem vorhandenen Rechner.

Ressourcenbeschränkungen können dort experimentell nachgebildet werden.

## 14. Programmiersprache

**Aktuelle Tendenz: C++**

C++ wird aufgrund seiner Performance, Hardwarekontrolle und Eignung für eingebettete Systeme bevorzugt.

Rust bleibt eine mögliche Alternative.

Python soll nicht für den eigentlichen neuronalen Kern eingesetzt werden, kann aber für Benutzeroberflächen, Experimentsteuerung und Auswertung dienen.

Eine endgültige Sprachentscheidung wurde noch nicht getroffen.

## 15. Erste experimentelle Zielsetzung

Das erste Netzwerk soll nicht Intelligenz demonstrieren, sondern überprüfbare grundlegende Gedächtniseigenschaften.

### Experiment A – Speicherung

Kann ein Eingangsmuster eine dauerhafte Veränderung im neuronalen System bewirken?

### Experiment B – Abruf und Wiedererkennung

Kann ein späteres Eingangsmuster aufgrund früherer Erfahrung eine veränderte Reaktion hervorrufen?

### Experiment C – Strukturelle Veränderung

Können sich Verbindungen und gegebenenfalls Neuronen verändern, neu bilden oder verschwinden?

### Experiment D – Ressourcenbegrenzung

Wie verändert sich das Verhalten des Netzwerks unter unterschiedlichen Speicher- und Rechenlimits?

### Experimentelle Anforderungen

- Reproduzierbare Anfangsbedingungen.
- Protokollierung neuronaler Ereignisse.
- Messung von Zustands- und Strukturänderungen.
- Vergleich von Reaktionen vor und nach Erfahrungen.
- Trennung von angeborenen Mechanismen und erlernten Veränderungen.
- Keine versteckten semantischen Vorgaben im neuronalen Kern.

## 16. Offene Forschungsfragen

1. Welche minimale Verschaltung ermöglicht ein dauerhaft veränderliches und abrufbares Gedächtnis?
2. Welche Eigenschaften muss ein einzelnes künstliches Neuron tatsächlich besitzen?
3. Wie werden synaptische Gewichte dargestellt und verändert?
4. Welche lokalen Regeln ermöglichen Lernen ohne vorgegebene semantische Bewertung?
5. Wie entstehen neue Verbindungen zwischen bisher unverbundenen Neuronen?
6. Unter welchen Bedingungen entstehen zusätzliche Neuronen?
7. Wie können bestehende Strukturen bei Ressourcenknappheit verändert oder abgebaut werden?
8. Können dieselben Mechanismen sowohl Lernen als auch Meta-Lernen ermöglichen?
9. Welche initialen Funktionen müssen angeboren sein?
10. Welche Rolle übernimmt das Stammhirn bei der neuronalen Entwicklung?
11. Wie lassen sich zeitabhängige Vorgänge vollständig ereignisgesteuert abbilden?
12. Wie kann nachgewiesen werden, dass beobachtete Verhaltensänderungen tatsächlich auf Erfahrung beruhen?

## 17. Zusammenfassender Grundsatz

EVE soll auf der niedrigsten Ebene mit binären neuronalen Impulsen arbeiten.

Neuronen besitzen veränderliche interne Zustände und reagieren asynchron auf Ereignisse.

Aus ihren Verbindungen, Gewichten, zeitlichen Zusammenhängen und strukturellen Veränderungen sollen Lernen, Erinnern und Wiedererkennen entstehen können.

Wir dürfen notwendige biologische Grundfunktionen und initiale Strukturen vorgeben.

Wir wollen jedoch weder fertige Erinnerungen noch semantische Bedeutungen oder vermeintlich richtige Entscheidungen in den neuronalen Kern einprogrammieren.

**Wir definieren die Voraussetzungen und untersuchen, was daraus tatsächlich entsteht.**

Dieses Dokument beschreibt den bisherigen Diskussionsstand. Es ist keine fertige neuronale Architektur und enthält ausdrücklich noch keine bestätigten experimentellen Ergebnisse.
