# Entwicklungsauftrag 035 – Forschungsstufe B.5.11

## Neuronale Repräsentation, Lerninterferenz, strukturelle Wiederverwendung und Validität unserer Bewertungsmethoden

**Projekt:** EVE – Emergent Virtual Evolution  
**Technologie:** C++17  
**Ausgangsbasis:** Konsolidierter Forschungsstand B.5.10 auf `main`  
**Forschungsbranch:** `codex/research-stage-b511-representation-and-learning-interference`  
**Auftragstyp:** Grundlagenforschung und kritische Überprüfung bisheriger Forschungsannahmen  
**Neue neuronale Produktionsregel:** Nicht freigegeben

---

## 1. Grundlegende Neuausrichtung

B.5.11 ersetzt die bisher geplante Untersuchung eines lokalen Selektors für synaptische Schreiboperationen.

Die vorausgegangenen Forschungsstufen haben wesentlich dazu beigetragen, EVEs neuronale Verarbeitung, synaptische Plastizität und zeitliche Dynamik zu verstehen.

Allerdings besteht der begründete Verdacht, dass unsere bisherigen Bewertungsmethoden neuronales Lernen zu eng interpretieren.

Wir haben häufig untersucht, ob eine bereits erworbene Reiz-Handlungs-Beziehung durch weitere Gewichtsänderungen erhalten bleibt oder beeinträchtigt wird.

Dabei wurde eine entscheidende Möglichkeit nicht ausreichend berücksichtigt:

**Das Lernen einer zusätzlichen Fähigkeit kann Veränderungen an bereits für andere Fähigkeiten verwendeten neuronalen Strukturen erforderlich machen.**

Eine solche Veränderung muss kein Vergessen darstellen.

Sie kann Bestandteil einer Reorganisation sein, durch die dasselbe neuronale Netzwerk anschließend mehrere Fähigkeiten beherrscht.

### Verbindliche Forschungsleitlinie

**Lernen können ist eine Fähigkeit. Etwas gelernt haben ist ein Zustand.**

Ergänzend gilt:

**Erlernte Fähigkeiten sind nicht mit unveränderlichen Synapsenkonfigurationen gleichzusetzen. Sie sind funktionale Eigenschaften eines neuronalen Systems.**

Die Forschung soll deshalb nicht mehr primär untersuchen, welche einzelnen Gewichte erhalten bleiben müssen.

Sie soll untersuchen, wie sich die Fähigkeiten eines neuronalen Systems entwickeln, wenn neue Informationen in bereits vorhandene neuronale Strukturen integriert werden.

---

## 2. Architekturhypothese: Wiederverwendung statt redundanter Informationsablage

Eine zentrale Arbeitshypothese dieses Forschungsauftrags lautet:

**Effizientes neuronales Lernen kann dadurch entstehen, dass vorhandene neuronale Strukturen für mehrere Fähigkeiten wiederverwendet, verändert und neu kombiniert werden.**

Eine neue Information muss nicht zwingend eine zusätzliche, unabhängige Gruppe von Synapsen besitzen.

Bereits bestehende Verbindungen können durch weitere Erfahrungen neue funktionale Rollen erhalten.

Dies betrifft sowohl:

- Die Gewichte vorhandener Synapsen.
- Die Beteiligung einzelner Neuronen an unterschiedlichen Aktivitätsmustern.
- Die gemeinsamen Pfade mehrerer Fähigkeiten.
- Die zeitliche Reihenfolge neuronaler Aktivität.
- Die Zusammenarbeit erregender und hemmender Beiträge.
- Gegebenenfalls die spätere strukturelle Veränderung des Netzwerks.

### Abgrenzung zur redundanten Speicherung

Ein System, das jede neue Information ausschließlich durch das Anlegen einer unabhängigen Kopie bereits vorhandener Informationsbestandteile speichert, wäre möglicherweise unnötig ressourcenintensiv.

Das entspräche eher einer einfachen Ablagestruktur als einem lernenden neuronalen System, das gemeinsame Strukturen nutzt.

Allerdings darf die Forschung nicht voraussetzen, dass jede Redundanz schädlich ist.

Redundanz kann unter anderem für Fehlertoleranz, Stabilität oder parallele Verarbeitung nützlich sein.

Deshalb müssen drei Möglichkeiten experimentell unterschieden werden:

1. Funktional notwendige gemeinsame neuronale Strukturen.
2. Funktional sinnvolle unabhängige oder redundante Strukturen.
3. Redundante Speicherung ohne nachgewiesenen zusätzlichen funktionalen Nutzen.

**Wiederverwendung ist eine Forschungsanforderung an die Untersuchung, kein vorab als erfolgreich festgelegtes Ergebnis.**

Es darf keine automatische technische Deduplizierung neuronaler Verbindungen eingeführt werden.

---

## 3. Das Lautsprecherprinzip

Als konzeptionelles Beispiel dient die Ausgabe von Schallwellen.

Ein Lautsprecher erzeugt zu einem gegebenen Zeitpunkt eine resultierende Membranauslenkung.

Der zeitliche Verlauf dieser Auslenkung kann jedoch unterschiedlichste Informationen transportieren.

Die physische Ausgabeeinheit benötigt dafür nicht für jede Information eine eigene, dauerhaft getrennte Membran.

Übertragen auf EVE:

Ein neuronaler Ausgang muss nicht für jede erlernte Information eine eigenständige neuronale Struktur besitzen.

Dieselbe neuronale Verbindung oder derselbe Aktivitätspfad kann an unterschiedlichen Informationen beteiligt sein.

Welche Information aktuell verarbeitet oder abgerufen wird, kann vom zeitlichen Zustand und den eintreffenden Eingängen abhängen.

Daraus folgt:

**Ein neuronales System muss nicht alle erlernten Fähigkeiten gleichzeitig ausdrücken können, um sie zu besitzen.**

Es muss aber unter geeigneten Bedingungen nachweislich auf diese Fähigkeiten zugreifen können.

B.5.11 muss deshalb zeitliche Verarbeitung und kontextabhängigen Abruf ausdrücklich untersuchen.

---

## 4. Zentrale Forschungsfragen

B.5.11 untersucht fünf grundlegende Fragen.

### A – Repräsentation

Wie werden mehrere erlernte Fähigkeiten innerhalb derselben neuronalen Struktur repräsentiert?

### B – Wiederverwendung

Werden vorhandene Neuronen und Synapsen für neue Fähigkeiten funktional wiederverwendet?

### C – Reorganisation

Können Veränderungen an bestehenden Verbindungen eine neue Fähigkeit ermöglichen, obwohl sich der unmittelbare Abruf einer früheren Fähigkeit zunächst verschlechtert?

### D – Bewertung

Messen unsere bisherigen Verfahren tatsächlich Lernen und Fähigkeitserhalt, oder überwiegend momentane Verhaltensleistung unter eng festgelegten Prüfbedingungen?

### E – Architektureinfluss

Behindern bestimmte bisher eingeführte neuronale Mechanismen die Integration, Wiederverwendung oder zeitliche Verarbeitung mehrerer Fähigkeiten?

**Vor Abschluss dieser Untersuchungen wird keine weitere synaptische Selektionsregel entwickelt.**

---

## 5. Bisherige Forschungsergebnisse und ihre Grenzen

### 5.1 B.5.3 – Transiente Membranzustände

B.5.3 zeigte, dass vorhandene Membranpotenziale den autonomen Abruf wesentlich beeinflussen können.

Daraus folgt:

Eine aktuell falsche Antwort ist kein hinreichender Nachweis, dass die synaptische Repräsentation einer Fähigkeit verloren gegangen ist.

### 5.2 B.5.4 – Recovery

Unabhängige Recovery-Mechanismen verbesserten bestimmte Abruf- und Robustheitswerte.

Nicht nachgewiesen wurde, ob diese Mechanismen die zeitliche Integration komplexerer Erfahrungen erhalten oder beeinträchtigen.

### 5.3 B.5.5 – S4

Die experimentelle Subthreshold-Relaxation verbesserte viele der bisherigen X/Y-Ergebnisse.

Sie entfernt jedoch nach einer festgelegten Zeit lokale unterschwellige Membranpotenziale.

Ob damit auch nützliche zeitlich verteilte Information verloren geht, ist offen.

S4 bleibt standardmäßig deaktiviert.

### 5.4 B.5.6 bis B.5.7 – Synaptische Interferenz

Sensorische Gewichtsänderungen wurden als wesentliche Ursache momentaner Abrufverluste identifiziert.

Die Untersuchungen haben jedoch nicht geklärt, ob einige dieser Veränderungen notwendige Zwischenschritte späterer gemeinsamer Fähigkeiten darstellen.

### 5.5 B.5.8 – Fähigkeitserhalt

In der untersuchten Stichprobe blieben 90/96 Netze bei unterdrückten Gewichtsschreibungen robust, gegenüber 62/96 bei aktiver Plastizität.

Nach Umweltumkehr lernten weiterhin gesperrte Netze nicht.

Dieser Befund zeigt:

Vorhandene Fähigkeiten benötigen nicht zwangsläufig fortwährende synaptische Änderungen.

Er beweist nicht, dass unveränderte Gewichte eine geeignete Strategie zum Erwerb zusätzlicher Fähigkeiten sind.

### 5.6 B.5.9 und B.5.10 – Einzelne Gewichtsänderungen

Die Forschungen ermöglichten die isolierte Untersuchung konkreter synaptischer Veränderungen.

B.5.10 untersuchte 119 prospektiv ausgewählte Schreibereignisse:

- 21 unmittelbar notwendige.
- 98 unmittelbar neutrale.
- Davon später drei nützlich und elf schädlich.
- 84 im Beobachtungsfenster weiterhin neutral.

Diese Kategorien wurden anhand bestimmter Verhaltensprüfungen ermittelt.

**Sie sind keine vollständige Aussage über die Bedeutung der Veränderungen für beliebige zukünftige Lernvorgänge.**

---

## 6. Kritische Überprüfung unserer bisherigen Bewertungslogik

### 6.1 Bisherige Grundannahme

Eine Beziehung wird als erfolgreich betrachtet, wenn bei einem bestimmten sensorischen Eingang die erwartete eindeutige Motorantwort entsteht.

### 6.2 Problem der momentanen Abrufbarkeit

Eine fehlende Antwort kann verschiedene Ursachen besitzen:

- Die Fähigkeit wurde noch nicht erworben.
- Die Fähigkeit wurde synaptisch beeinträchtigt.
- Ein vorübergehender Membranzustand verhindert den Abruf.
- Die aktuelle neuronale Aktivität ist auf einen anderen Zusammenhang ausgerichtet.
- Ein zeitlicher Eingang fehlt.
- Eine notwendige Aktivitätsfolge wurde nicht abgeschlossen.

Diese Zustände dürfen nicht automatisch gleichgesetzt werden.

### 6.3 Problem der Summenbewertung

Wenn A korrekt und B falsch ist, ergibt sich in unserer einfachen Paarbewertung derselbe Wert wie bei A falsch und B korrekt.

Eine neuronale Veränderung kann damit als neutral erscheinen, obwohl sich das tatsächlich verfügbare Verhalten vollständig geändert hat.

### 6.4 Problem der isolierten Fähigkeiten

Unsere bisherigen Tests bevorzugen die unabhängige Abrufbarkeit von A und B aus identischen Ausgangszuständen.

Ein neuronales System könnte jedoch zusätzliche Aktivität oder Kontext benötigen, um unterschiedliche Fähigkeiten über dieselben Verbindungen auszudrücken.

### 6.5 Problem des unmittelbaren Erfolgs

Eine synaptische Veränderung kann kurzfristig eine Antwort verschlechtern und trotzdem notwendig sein, um später eine zusätzliche Fähigkeit zu erwerben.

### Forschungsanforderung

Die bisherige E1-/E2-Bewertung muss erhalten bleiben, darf jedoch nicht länger der einzige Maßstab für erfolgreiche neuronale Entwicklung sein.

---

## 7. Neue Bewertungsdimensionen

B.5.11 führt zunächst ausschließlich auf Forschungsebene mehrere getrennte Bewertungsdimensionen ein.

### V1 – Momentane Verhaltensleistung

Welche Motorantwort entsteht unter den aktuellen Bedingungen?

### V2 – Bedingter Fähigkeitsabruf

Kann eine zuvor erworbene Fähigkeit unter unterschiedlichen gültigen Eingangskontexten abgerufen werden?

### V3 – Zeitliche Abrufbarkeit

Wie hängt der Abruf von der Reihenfolge und vom zeitlichen Abstand vorheriger Ereignisse ab?

### V4 – Repertoire

Welche unterschiedlichen Fähigkeiten kann dasselbe neuronale Netz unter jeweils passenden Bedingungen ausführen?

### V5 – Lernfortschritt

Welche neuen funktionalen Möglichkeiten entstehen durch zusätzliche Erfahrungen?

### V6 – Retention

Welche früher erworbenen Möglichkeiten bleiben später verfügbar?

### V7 – Wiedererwerb

Wie verändert sich die erneute Erwerbsfähigkeit einer vorübergehend schlechter abrufbaren Beziehung?

### V8 – Transfer

Kann eine zuvor erworbene Struktur den Erwerb einer verwandten neuen Fähigkeit erleichtern?

### V9 – Repräsentationswiederverwendung

Welche neuronalen Komponenten tragen kausal zu mehreren Fähigkeiten bei?

### V10 – Ressourceneffizienz

Welcher funktionale Fähigkeitsumfang entsteht bei gegebener Anzahl von Neuronen, Synapsen, Ereignissen und Gewichtsänderungen?

Diese Dimensionen sind zunächst getrennt zu protokollieren.

**Sie dürfen nicht durch einen beliebig gewichteten globalen Lernscore ersetzt werden.**

---

## 8. Phase A – Forschungsbasis und unveränderte Referenz

### A1 – Branch

Erstelle den neuen Forschungsbranch vom vollständig integrierten B.5.10-Stand auf `main`.

### A2 – Regression

Reproduziere mindestens die zentralen Referenzwerte:

- B.5.7 Discovery: 242/384.
- B.5.7 Bestätigung: 254/384.
- B.5.8/B.5.9 Discovery: 240/384.
- B.5.8/B.5.9 Bestätigung: 244/384.
- B.5.10 Einzel-Schreibinterventionen.

### A3 – Unveränderte Produktionslogik

Die bestehenden neuronalen Regeln bleiben für die primären Experimente unverändert.

### A4 – Diagnoseschnittstellen

Die vorhandenen Beobachter und Schreibinterventionen dürfen für kausale Gegenproben verwendet werden.

Sie dürfen nicht zur automatischen Stabilisierung erfolgreicher Verbindungen eingesetzt werden.

---

## 9. Phase B – Neue Lernumgebung mit geteilten Informationsbestandteilen

Die bisherige binäre X/Y-Aufgabe reicht nicht aus, um die Wiederverwendung neuronaler Informationsbestandteile zuverlässig zu untersuchen.

B.5.11 benötigt deshalb eine erweiterte, aber weiterhin kleine und deterministisch reproduzierbare Forschungsumgebung.

### B1 – Gemeinsame Bestandteile

Definiere abstrakte sensorische Elemente, die in mehreren Aufgaben wiederkehren.

Beispielsweise:

- Element P.
- Element Q.
- Element R.

Diese Bezeichnungen existieren ausschließlich in der externen Versuchsbeschreibung.

### B2 – Zeitliche Muster

Erzeuge Erfahrungen, die aus unterschiedlichen Kombinationen derselben Eingangselemente bestehen.

Beispielsweise:

- Aufgabe A verwendet die Folge P → Q.
- Aufgabe B verwendet die Folge P → R.
- Eine zusätzliche Transferaufgabe verwendet eine neue Kombination bereits bekannter Elemente.

Die Aufgaben teilen sich damit einzelne sensorische Bestandteile.

### B3 – Keine semantischen Abkürzungen

EVE darf nicht direkt mitgeteilt bekommen:

- Dass P in mehreren Aufgaben dieselbe Bedeutung besitzt.
- Dass A und B verwandt sind.
- Welche Teilstruktur wiederverwendet werden soll.
- Welche Motorantwort für eine Aufgabe technisch richtig ist.

Die äußere Umgebung darf tatsächliche Handlungen bewerten.

Die neuronale Verarbeitung darf ausschließlich die vorhandenen sensorischen Signale und regulären Modulationswege verwenden.

### B4 – Unabhängige Kontrollaufgaben

Erstelle zusätzlich Aufgaben ohne gemeinsam verwendete sensorische Bestandteile.

### B5 – Vergleich

Prüfe, ob gemeinsame Bestandteile den Erwerb weiterer Aufgaben erleichtern oder erschweren.

### B6 – Schwierigkeit

Die Aufgaben sind so zu gestalten, dass einfache fest verdrahtete Lösungen und triviale Unterschiede der Eingangshäufigkeit nicht die Ergebnisse bestimmen.

Die genaue Aufgabendefinition ist vor der Discovery-Auswertung festzulegen.

---

## 10. Phase C – Lernverlauf A → B

### C1 – Erwerb von A

EVE erhält natürliche Erfahrungen mit Aufgabe A.

### C2 – Zustand nach A

Erfasse:

- Neuronale Gewichte.
- Tatsächliche Feuerfolgen.
- Motorantworten.
- Abrufbarkeit.
- Eligibility.
- Membran- und Recovery-Zustände.

### C3 – Erwerb von B

Anschließend erhält dasselbe Netz Erfahrungen mit Aufgabe B.

Es erfolgt kein Zurücksetzen.

### C4 – Veränderungen während B

Untersuche:

- Welche bereits für A aktiven Neuronen erneut verwendet werden.
- Welche vorhandenen Synapsen verändert werden.
- Welche neuen Aktivitätsfolgen entstehen.
- Welche A-bezogenen Verbindungen an B beteiligt sind.
- Welche A-bezogenen Veränderungen auch für B funktional notwendig sind.

### C5 – Momentane Leistung

Prüfe A und B getrennt über vollständige Zustandskopien.

### C6 – Interpretation

Ein vorübergehender Rückgang der A-Leistung wird nicht automatisch als endgültiges Vergessen klassifiziert.

Er wird zunächst als veränderte Abrufbarkeit dokumentiert.

---

## 11. Phase D – Lernverlauf A → B → A

### D1 – Erster Erwerb

EVE erwirbt A.

### D2 – Zweiter Erwerb

EVE erhält Erfahrungen mit B.

### D3 – Erneute A-Erfahrung

EVE erhält erneut Erfahrungen mit A.

### D4 – Forschungsfragen

- Wird A schneller wieder verfügbar als beim erstmaligen Erwerb?
- Sind dafür weniger synaptische Veränderungen notwendig?
- Wurden für A relevante Verbindungen zwischenzeitlich für B verwendet?
- Verändert sich B während des erneuten A-Erwerbs?
- Ist der erneute Erwerb Ausdruck fortbestehender Repräsentation oder vollständigen Neulernens?

### D5 – Kontrollgruppen

Vergleiche mit:

- Einem erstmaligen A-Lernverlauf ohne vorheriges A.
- Einem A-only-Verlauf mit gleicher Gesamtdauer.
- Einem B-only-Verlauf.
- Einer Kontrollgruppe mit gleicher Anzahl Erfahrungen, aber unabhängigen Eingängen.

### D6 – Keine automatische Erinnerung

Ein schnellerer Wiedererwerb ist ein Hinweis auf vorhandene funktionale Vorstrukturierung.

Er beweist nicht allein eine bestimmte gespeicherte Repräsentation.

---

## 12. Phase E – Lernverlauf A → B → A+B

Diese Phase untersucht ausdrücklich die Integration mehrerer Fähigkeiten.

### E1 – Sequentieller Erwerb

EVE erwirbt zunächst A, anschließend B.

### E2 – Gemischte natürliche Erfahrungen

Danach erhält EVE Erfahrungen mit beiden Aufgaben in unterschiedlicher Reihenfolge.

### E3 – Gemeinsame Verfügbarkeit

Prüfe, ob die einzelnen Aufgaben unter geeigneten Bedingungen abgerufen werden können.

### E4 – Zeitliche Ausgaben

Eine gleichzeitige motorische Ausgabe beider Fähigkeiten wird nicht vorausgesetzt.

Die korrekte Ausgabe richtet sich nach der jeweiligen sensorischen Situation und ihrem zeitlichen Kontext.

### E5 – Neuorganisation

Untersuche, welche Verbindungen während der gemischten Erfahrungen erneut verändert werden.

### E6 – Kausaler Nachweis

Prüfe, ob diese Veränderungen für die spätere gemeinsame Verfügbarkeit erforderlich sind.

### E7 – Wichtigste Frage

**Kann eine Änderung, die A während des B-Erwerbs zunächst beeinträchtigt, später zur gemeinsamen Fähigkeit A+B beitragen?**

---

## 13. Phase F – Interleaved Learning

Neben sequentiellem Erwerb muss EVE auch wechselnde Aufgabenfolgen verarbeiten.

### F1 – Vergleichsbedingungen

- Erst A, dann B.
- Erst B, dann A.
- Abwechselnd A und B.
- Zufällig gemischte A-/B-Erfahrungen.
- Unterschiedlich lange zusammenhängende Aufgabenblöcke.

### F2 – Gleiche Erfahrungsmenge

Die Gesamtanzahl der präsentierten Aufgaben und die Verteilung der sensorischen Bestandteile müssen kontrolliert werden.

### F3 – Forschungsfrage

Welche Reihenfolgen fördern oder behindern:

- Wiederverwendung.
- Gemeinsame Abrufbarkeit.
- Stabilität.
- Wiedererwerb.
- Ressourceneffizienz?

### F4 – Keine technische bevorzugte Lernreihenfolge

Die neuronale Architektur darf keine hart codierten A-/B-Lernphasen erhalten.

---

## 14. Phase G – Transfer auf eine dritte Fähigkeit

### G1 – Aufbau

Definiere eine dritte Aufgabe C, die einzelne bereits vorhandene sensorische Bestandteile wiederverwendet, aber eine neue funktionale Anforderung besitzt.

### G2 – Vergleich

Untersuche den Erwerb von C:

- Ohne vorheriges Lernen.
- Nach A.
- Nach B.
- Nach A+B.
- Nach unabhängigen Kontrollaufgaben mit gleicher Erfahrungshäufigkeit.

### G3 – Transfer

Ein positiver Transfer liegt vor, wenn frühere Erfahrungen den Erwerb oder die spätere Leistung von C gegenüber geeigneten Kontrollen verbessern.

### G4 – Negativer Transfer

Untersuche ebenso, ob vorherige Fähigkeiten den Erwerb von C erschweren.

### G5 – Repräsentationsfrage

Welche bereits vorhandenen neuronalen Komponenten werden bei C tatsächlich erneut verwendet?

### G6 – Keine bloße Wiederholung

Die Aufgabe C darf nicht lediglich eine identische Wiederholung von A oder B sein.

---

## 15. Phase H – Gemeinsame neuronale Repräsentation messen

**Diese Phase ist ein zentraler Teil des Forschungsauftrags.**

### H1 – Aktive Neuronen

Erfasse, welche Neuronen bei A, B und C aktiv werden.

### H2 – Aktive Synapsen

Erfasse, welche Synapsen an den jeweiligen Verarbeitungsfolgen beteiligt sind.

### H3 – Überlappung

Bestimme, welche Komponenten bei mehreren Aufgaben vorkommen.

### H4 – Funktionale Wiederverwendung

Eine gemeinsame Aktivierung allein ist kein ausreichender Beweis für gemeinsame Repräsentation.

Prüfe deshalb kausal, ob die betreffende Komponente bei mehreren Fähigkeiten funktional wirksam ist.

### H5 – Einzelablationen

Entferne in kontrollierten Zustandskopien einzelne Beiträge.

### H6 – Kombinierte Ablationen

Untersuche redundante und gemeinsam notwendige Verbindungen.

### H7 – Unspezifische Engpässe

Unterscheide echte gemeinsame Repräsentationen von allgemeinen Ein- und Ausgabepfaden, deren Ausfall schlicht alle Aufgaben beeinträchtigt.

### H8 – Wiederverwendung während Lernen

Prüfe, ob bestehende Verbindungen ihre funktionale Rolle ändern, während eine zusätzliche Fähigkeit erworben wird.

### H9 – Ergebnis

Ordne neuronale Komponenten diagnostisch ein als:

- Vorwiegend A-bezogen.
- Vorwiegend B-bezogen.
- Für mehrere Aufgaben funktional beteiligt.
- Allgemeiner Verarbeitungs- oder Ausgangspfad.
- Im untersuchten Fenster ohne nachgewiesene funktionale Beteiligung.

Diese Einordnung darf keine neue semantische Speicherung innerhalb von EVE erzeugen.

---

## 16. Phase I – Reorganisation vorhandener Synapsen

### I1 – Gewichtstrajektorien

Verfolge bestehende Synapsen vom Erwerb von A über B bis zur gemeinsamen Abrufbarkeit.

### I2 – Veränderte Funktionen

Untersuche, ob dieselbe Synapse bei unterschiedlichen Aufgaben unterschiedlich zur Aktivität beiträgt.

### I3 – Erregung und Hemmung

Prüfe Veränderungen im Zusammenspiel erregender und hemmender Verbindungen.

### I4 – Zeitliche Funktion

Untersuche, ob eine bestehende Synapse bei unterschiedlichen zeitlichen Aktivitätsfolgen verschiedene funktionale Beiträge leistet.

### I5 – Kausale Intervention

Vergleiche die natürliche Gewichtsänderung mit ihrem isolierten Auslassen.

### I6 – Langfristige Folge

Verfolge die Wirkung nicht nur unmittelbar, sondern über weitere Erfahrungen mit A und B.

### I7 – Reorganisationsnachweis

Eine Veränderung kann als Bestandteil funktionaler Reorganisation gelten, wenn ihr kausaler Einfluss auf die Integration oder den späteren gemeinsamen Abruf mehrerer Fähigkeiten nachgewiesen wird.

Eine reine Gewichtsänderung ohne diesen Nachweis genügt nicht.

---

## 17. Phase J – Lerninterferenz neu definieren

Die bisherigen Begriffe „nützlich“, „neutral“ und „schädlich“ müssen für diese Forschungsstufe präzisiert werden.

### J1 – Vorübergehende Abrufbeeinträchtigung

Eine zuvor funktionierende Aufgabe ist zeitweise schlechter abrufbar.

### J2 – Langfristige Beeinträchtigung

Eine Fähigkeit bleibt auch unter geeigneten späteren Abrufbedingungen eingeschränkt.

### J3 – Erleichterter Wiedererwerb

Eine beeinträchtigte Fähigkeit wird schneller wieder verfügbar als beim erstmaligen Erwerb.

### J4 – Produktive Reorganisation

Eine zunächst beeinträchtigende Veränderung ist kausal an einer späteren erweiterten Fähigkeit beteiligt.

### J5 – Negative Lerninterferenz

Der Erwerb einer neuen Fähigkeit verursacht eine längerfristige Einschränkung anderer Fähigkeiten, ohne dass im geprüften Repertoire ein kompensierender funktionaler Nutzen nachgewiesen wird.

### J6 – Ungeklärte Veränderung

Die Wirkung kann innerhalb des Beobachtungsfensters nicht eindeutig bewertet werden.

### Verbindliche Regel

**Eine vorübergehend falsche Antwort darf nicht automatisch als schädliche synaptische Veränderung klassifiziert werden.**

Ebenso darf eine vorübergehende Beeinträchtigung nicht ohne Nachweis als notwendige Reorganisation gerechtfertigt werden.

---

## 18. Phase K – Zeitliche neuronale Repräsentation

### K1 – Einzelner Eingang

Prüfe die Reaktion auf einen isolierten sensorischen Reiz.

### K2 – Eingangsfolge

Prüfe dieselben sensorischen Elemente in unterschiedlicher zeitlicher Reihenfolge.

### K3 – Gleiche Elemente, andere Bedeutung

Untersuche, ob unterschiedliche Reihenfolgen bei identischen Bestandteilen zu unterschiedlichen erlernten Verhaltensweisen führen können.

### K4 – Zeitlicher Kontext

Prüfe, wie vorherige Aktivität die aktuelle Antwort beeinflusst.

### K5 – Keine permanente Parallelverfügbarkeit voraussetzen

Ein Netzwerk darf auf unterschiedliche zeitliche Kontexte mit unterschiedlichen Antworten reagieren.

### K6 – Unterscheidung

Untersuche, ob die Informationen durch:

- Synaptische Gewichte.
- Transiente Membranzustände.
- Wiederkehrende Aktivitätsfolgen.
- Gemeinsame synaptische Pfade.

getragen werden.

Diese Ursachen dürfen nicht ohne kausale Gegenproben gleichgesetzt werden.

---

## 19. Phase L – Überprüfung von R0, R2 und S4

**Diese Phase ist verpflichtend.**

Bisherige Verbesserungen der Abrufrobustheit könnten bestimmte Formen zeitlicher Informationsverarbeitung beeinträchtigen.

### L1 – Historische Referenz R0

Untersuche die ursprüngliche Membrandynamik unter den neuen Mehrfachlernaufgaben.

### L2 – R2

Untersuche die vorhandene unabhängige Recovery-Variante.

### L3 – S4-d16

Untersuche die experimentelle Subthreshold-Relaxation.

### L4 – Identische Bedingungen

Vergleiche:

- Dieselben Forschungsaufgaben.
- Dieselben Seeds.
- Dieselben Erfahrungsmengen.
- Dieselben zeitlichen Umgebungen.
- Dieselben Ressourcenbedingungen.

Die tatsächlich konfigurierte Refraktär- und Recovery-Verarbeitung muss dokumentiert werden.

### L5 – Neue Messgrößen

Untersuche:

- Erwerb von A.
- Erwerb von B.
- Gemeinsame Abrufbarkeit.
- Wiedererwerb von A.
- Transfer zu C.
- Zeitliche Musterunterscheidung.
- Funktionale Wiederverwendung.
- Lerninterferenz.

### L6 – S4 als möglicher Störfaktor

Prüfe, ob S4 nützliche unterschwellige Aktivität entfernt, die für zeitlich verteilte Informationen benötigt wird.

### L7 – R2 als möglicher Störfaktor

Prüfe, ob Recovery zeitliche Aktivitätsfolgen beeinflusst.

### L8 – Ergebnisoffenheit

Ein Mechanismus, der die bisherige X/Y-Robustheit verbessert, kann unter einer komplexeren Aufgabe:

- Hilfreich sein.
- Neutral sein.
- Schädlich sein.

Eine positive historische Bewertung darf kein neues Ergebnis vorwegnehmen.

S4 bleibt standardmäßig deaktiviert.

---

## 20. Phase M – Auswirkungen der bisherigen Plastizität

Untersuche, ob die vorhandene lokale Lernregel eine gemeinsame Reorganisation ermöglicht oder bestimmte Formen der Wiederverwendung behindert.

### M1 – Eligibility

Prüfe, ob die aktuelle Eligibility-Auswahl gemeinsame synaptische Pfade für mehrere Aufgaben ausreichend berücksichtigt.

### M2 – Nichtfeuernde Verarbeitung

Prüfe, welche funktional relevanten unterschwelligen Vorgänge derzeit keine Eligibility erzeugen.

### M3 – Negative Modulation

Untersuche, ob negative Rückmeldung während B notwendige A/B-Reorganisation erschwert oder unterstützt.

### M4 – Positive Modulation

Untersuche, ob positive Verstärkung bereits verwendeter Verbindungen eine zusätzliche Fähigkeit ermöglicht.

### M5 – Konkurrenz

Prüfe den Einfluss von Gewichtsgrenzen und relativer erregender bzw. hemmender Konkurrenz.

### M6 – Isolierte Schreibinterventionen

Verwende den vorhandenen Forschungs-Interceptor, um konkrete Änderungen kausal zu untersuchen.

### M7 – Keine neue Regel

Die Untersuchung gibt weder eine neue Eligibility-Regel noch einen lokalen Schreibselektor frei.

---

## 21. Phase N – Feste Topologie und strukturelle Veränderung

Die bisherigen Forschungsnetze verwendeten eine kleine, weitgehend festgelegte Struktur.

In den zentralen Referenzversuchen war strukturelles Synapsenwachstum deaktiviert.

### N1 – Bestehende Synapsen

Untersuche zunächst, wie weit vorhandene Verbindungen durch Gewichtsänderungen für zusätzliche Fähigkeiten wiederverwendet werden können.

### N2 – Kapazitätsgrenzen

Prüfe, ob bestimmte Aufgaben wegen begrenzter neuronaler Ressourcen scheitern.

### N3 – Kontrollierte Netzgrößen

Vergleiche vorher festgelegte unterschiedliche Netzgrößen und Verbindungskapazitäten.

### N4 – Strukturelle Plastizität

Falls die vorhandenen Mechanismen mit fester Topologie eine relevante Grenze zeigen, darf eine getrennte, standardmäßig deaktivierte Forschungsvariante mit vorhandener struktureller Veränderbarkeit untersucht werden.

### N5 – Keine vorgefertigten Wissensstrukturen

Zusätzliche Synapsen oder Neuronen dürfen nicht anhand der extern bekannten richtigen Lösung angelegt werden.

### N6 – Keine unbegrenzte Vergrößerung

Mehr Struktur darf nicht als automatische Lösung angenommen werden.

### N7 – Vergleich

Untersuche, ob zusätzliche Fähigkeiten eher durch:

- Veränderung bestehender Gewichte.
- Wiederverwendung vorhandener Pfade.
- Neue Verbindungen.
- Kombination dieser Prozesse.

entstehen.

Die tatsächlichen Befunde entscheiden über die weitere Architektur.

---

## 22. Phase O – Ressourceneffizienz und Redundanz

Eine neuronale Lernarchitektur muss nicht nur Fähigkeiten erwerben, sondern mit ihren Ressourcen sinnvoll umgehen.

### O1 – Ressourcengrößen

Erfasse:

- Anzahl Neuronen.
- Anzahl vorhandener Synapsen.
- Anzahl tatsächlich veränderter Gewichte.
- Anzahl wirksamer gemeinsamer Pfade.
- Anzahl zusätzlicher Verbindungen.
- Speicherbedarf.
- Ereignisverarbeitung.
- CPU-Zeit.
- Zusätzliche Aktivitätsereignisse.

### O2 – Fähigkeitsumfang

Erfasse, welche Aufgaben nach dem Training unter den geeigneten Prüfbedingungen verfügbar sind.

### O3 – Gemeinsame versus getrennte Strukturen

Vergleiche Netze, die vorhandene Ressourcen teilen können, mit geeigneten Kapazitätskontrollen für getrennte Ressourcen.

### O4 – Faire Vergleichsbasis

Wenn eine Kontrollarchitektur zusätzliche Neuronen oder Synapsen verwendet, ist dieser Ressourcenunterschied ausdrücklich auszuweisen.

### O5 – Funktionale Redundanz

Untersuche, ob redundante Verbindungen zusätzliche Robustheit oder Fehlertoleranz bieten.

### O6 – Keine künstliche Deduplizierung

Es darf keine Regel eingeführt werden, die ähnliche Synapsen oder Aktivitätsmuster allein aufgrund äußerer Ähnlichkeit zusammenfasst.

### O7 – Forschungsziel

Prüfe, ob die Wiederverwendung vorhandener neuronaler Strukturen zu einem größeren Fähigkeitsumfang bei vergleichbarem Ressourcenaufwand führt.

---

## 23. Phase P – Neue Evaluationsverfahren

Die bisherigen Prüfungen bleiben als Referenz erhalten.

Daneben sind neue, unabhängige Bewertungsverfahren zu entwickeln.

### P1 – Kontextabhängiger Abruf

Prüfe jede Fähigkeit unter mehreren gültigen sensorischen und zeitlichen Bedingungen.

### P2 – Getrennte vollständige Kopien

Verwende vollständige Runtime-Kopien, damit eine diagnostische Prüfung nicht unbemerkt den ursprünglichen Trainingsverlauf verändert.

### P3 – Aufeinanderfolgende Prüfungen

Prüfe zusätzlich mehrere Aufgaben hintereinander im selben fortlaufenden Zustand.

### P4 – Eingangsreihenfolge

Untersuche, ob A → B und B → A unterschiedliche Ergebnisse liefern.

### P5 – Unterschiedliche Startkontexte

Prüfe unter anderem:

- Nach einer A-Erfahrung.
- Nach einer B-Erfahrung.
- Nach einer kurzen ereignisfreien Zeit.
- Nach einer längeren ereignisfreien Zeit.
- Nach gemischten natürlichen Erfahrungen.

### P6 – Kein globaler Reset

Der normale Lern- und Abrufpfad darf nicht durch künstliche globale Zustandslöschungen verbessert werden.

### P7 – E1/E2

Die bestehenden Kennzahlen bleiben dokumentiert.

Sie werden jedoch durch die neuen Bewertungsdimensionen ergänzt.

### P8 – Vollständige Antwortmatrix

Erfasse für jede Aufgabe:

- Erwartete äußere Bewertung.
- Tatsächlich ausgeführte Handlung.
- Fehlende Antwort.
- Doppelantwort.
- Andere eindeutige Antwort.
- Zeitliche Abhängigkeit.

### P9 – Kein pauschaler Summenwert

A und B dürfen nicht ausschließlich zu einer Anzahl korrekter Beziehungen zusammengefasst werden.

Ein Tausch der verfügbaren Fähigkeiten muss als Verhaltensänderung erkennbar bleiben.

---

## 24. Phase Q – Kausale Untersuchung produktiver Reorganisation

Diese Phase prüft die zentrale Hypothese unmittelbar.

### Q1 – Kandidaten

Identifiziere Veränderungen, die:

1. Während des Erwerbs von B entstehen.
2. Den momentanen Abruf von A verändern.
3. Später möglicherweise zur gemeinsamen Verfügbarkeit von A und B beitragen.

### Q2 – Isolierte Gegenproben

Vergleiche vollständige Kopien mit:

- Natürlich ausgeführter Veränderung.
- Isoliert ausgelassener Veränderung.
- Unveränderter identischer Kontrollkopie.

### Q3 – Natürliche Fortsetzung

Alle Kopien erhalten weitere tatsächliche Erfahrungen mit A und B.

### Q4 – Prüffrage

Ermöglicht eine Veränderung, die A zunächst beeinträchtigt, später eine zusätzliche oder bessere gemeinsame Fähigkeit?

### Q5 – Notwendigkeit

Falls die gemeinsame Fähigkeit ohne die Veränderung nicht entsteht, kann dies einen kausalen Hinweis auf produktive Reorganisation liefern.

### Q6 – Gegenkontrolle

Untersuche ebenso Veränderungen, die A beeinträchtigen und auch langfristig keinen nachgewiesenen Nutzen besitzen.

### Q7 – Keine nachträgliche Rechtfertigung

Nicht jeder Abrufverlust darf als produktive Reorganisation ausgegeben werden.

Die spätere positive Wirkung muss nachgewiesen werden.

---

## 25. Phase R – Frühere Forschungsergebnisse neu einordnen

Erstelle eine ausdrückliche methodische Neubewertung der Forschungsstufen B.5.3 bis B.5.10.

### R1 – Weiterhin gültige Erkenntnisse

Welche bisherigen Befunde beschreiben tatsächliche lokale neuronale Mechanismen und bleiben unabhängig vom Bewertungsmaßstab gültig?

### R2 – Bewertungsabhängige Erkenntnisse

Welche Befunde beruhen auf der bisherigen Definition korrekter X/Y-Antworten?

### R3 – Neue Interpretation

Welche bislang als schädlich oder neutral eingestuften Gewichtsänderungen können unter den neuen Aufgaben anders eingeordnet werden?

### R4 – Reorganisation

Gibt es nachweislich produktive Zwischenschritte, die mit der bisherigen Bewertung als Verluste erschienen wären?

### R5 – Architekturfolgen

Welche vorhandenen Mechanismen bleiben sinnvoll, welche müssen weiter untersucht werden und welche könnten die neue Lernrichtung einschränken?

### R6 – Keine retrospektive Gewissheit

Ein neuer Bewertungsmaßstab darf frühere technische Kausalbefunde nicht pauschal für falsch erklären.

Es ist zwischen korrektem lokalem Befund und zu weitreichender Interpretation zu unterscheiden.

---

## 26. Kontrollgruppen

Mindestens:

| Gruppe | Untersuchung |
|---|---|
| K0 | Historische X/Y-Referenz |
| K1 | Nur A |
| K2 | Nur B |
| K3 | A → B |
| K4 | B → A |
| K5 | A → B → A |
| K6 | A → B → A+B |
| K7 | Abwechselnder A-/B-Erwerb |
| K8 | Zufällig gemischter Erwerb |
| K9 | Unabhängige Aufgaben ohne gemeinsame Bestandteile |
| K10 | Verwandte Aufgaben mit gemeinsamen Bestandteilen |
| K11 | Transfer auf C |
| K12 | Gleiche Gesamtzahl von Erfahrungen ohne vorherigen Transfer |
| K13 | Gemeinsame neuronale Pfade |
| K14 | Kausale Einzelablation |
| K15 | Kausale kombinierte Ablation |
| K16 | Natürliche synaptische Reorganisation |
| K17 | Isoliert ausgelassene Veränderung |
| K18 | R0 |
| K19 | R2-50 |
| K20 | S4-d16 |
| K21 | Feste Topologie |
| K22 | Kontrollierte alternative Netzkapazität |
| K23 | Gegebenenfalls experimentelle strukturelle Plastizität |
| K24 | No-Learning |
| K25 | Diagnose mit gesperrten Gewichtsschreibungen |
| K26 | Reaktivierte Plastizität |
| K27 | Historische E1-/E2-Bewertung |
| K28 | Neue kontext- und sequenzabhängige Bewertung |

Nicht jede Kombination muss ein vollständig unabhängiger großer Matrixlauf werden.

Die primären Vergleiche und die benötigten Fallzahlen sind vor der Ergebnisanalyse festzulegen.

---

## 27. Stichproben und unabhängige Bestätigung

### 27.1 Historische Referenzen

Die bisherigen B.5-Daten dienen als technische Reproduktionsreferenz.

### 27.2 Discovery

Mindestens:

- 64 Seeds je zentraler Zufallsfamilie und Zuordnung.
- Drei unabhängige Zufallsstromfamilien.
- Normale und inverse Zuordnungen, soweit für die jeweilige Aufgabe anwendbar.
- Vorab festgelegte Erfahrungsbudgets.
- Reproduzierbare natürliche Exploration.

### 27.3 Bestätigung

Mindestens weitere 64 bisher unbenutzte Seeds je zentraler Kombination.

### 27.4 Keine Doppelverwendung

Bestätigungsdaten dürfen nicht zur nachträglichen Auswahl günstiger Aufgaben, Architekturen, Zeitparameter oder Bewertungsgewichte verwendet werden.

### 27.5 Vergleichbarkeit

Die Aufgaben müssen in ihren Erfahrungsbudgets, Ressourcen und Prüfbedingungen möglichst vergleichbar sein.

### 27.6 Zulässige negative Befunde

Falls eine Aufgabenklasse mit den vorhandenen Ressourcen nicht erlernt werden kann, ist dies ein gültiger Befund.

Es darf kein gewünschter Lernerfolg durch technische Zielinformationen erzwungen werden.

---

## 28. Statistische Auswertung

Erforderlich sind:

- Gepaarte Vergleiche.
- Seedbasierte Clusterung.
- 95-%-Bootstrap-Konfidenzintervalle.
- Getrennte Ergebnisse je Aufgabe.
- Getrennte Ergebnisse je Lernreihenfolge.
- Getrennte Ergebnisse je Membranvariante.
- Vollständige Antwortmatrizen.
- Erstmaliger Erwerb.
- Wiedererwerb.
- Retention.
- Zeitliche Abrufbarkeit.
- Transfer auf C.
- Funktionale Pfadwiederverwendung.
- Ressourceneffizienz.
- Vorübergehende Abrufbeeinträchtigung.
- Langfristige Einschränkung.
- Produktive Reorganisation.

### Keine unzulässige Zusammenfassung

Eine höhere Gesamtsumme korrekter Antworten darf nicht automatisch als umfassenderes Lernen bezeichnet werden.

Mehrere Fähigkeiten müssen einzeln und in ihren gegenseitigen Abhängigkeiten ausgewertet werden.

### Kein vorab festgelegtes Optimierungsziel

Die bisherigen 75-%- und 48/64-Grenzen bleiben für ihre ursprünglichen Referenzaufgaben bestehen.

Neue Aufgaben benötigen eigene, vorab begründete Leistungskriterien.

Die ursprünglichen Schwellen dürfen nicht unbesehen auf andersartige Lernaufgaben übertragen werden.

---

## 29. Technische Architekturgrenzen

Unverändert gelten:

- C++17.
- Ereignisgesteuerte neuronale Verarbeitung.
- Keine periodische Aktualisierung sämtlicher Neuronen.
- Keine globale Lernentscheidung.
- Binäre Feuerereignisse.
- Gerichtete gewichtete Synapsen.
- Erregende und hemmende Verarbeitung.
- Lokales G3.
- Eligibility.
- Drei-Faktor-Plastizität.
- Geschützte angeborene Exploration.
- Keine technisch richtige Handlung innerhalb der neuronalen Lernlogik.
- Keine globale semantische Wissensablage.
- Keine automatisch geschützten erlernten Gewichte.
- Kein automatischer Rollback.
- Kein technischer Winner-take-all-Mechanismus.
- Kein künstlicher Reset nach jedem Lernabschnitt.

Die vorhandene experimentelle Gewichtsschreibsperre und der Einzel-Schreibinterceptor bleiben ausschließlich diagnostische Instrumente.

S4 bleibt standardmäßig deaktiviert.

Strukturelle Plastizität darf nur in getrennten, ausdrücklich gekennzeichneten Forschungskontrollen untersucht werden.

---

## 30. Persistenz, Determinismus und Ressourcen

### 30.1 Persistenz

Alle natürlichen Lernzustände müssen vollständig gespeichert und wiederhergestellt werden können.

### 30.2 Fortsetzung

Nach Laden muss die deterministische Fortsetzung dieselbe neuronale Entwicklung erzeugen.

### 30.3 Zeitliche Zustände

Dabei sind insbesondere zu prüfen:

- Membranpotenziale.
- Refraktärzustände.
- Eligibility.
- Recovery.
- S4, sofern experimentell aktiviert.
- Zukünftige Ereignisse.
- Synaptische Gewichte.

### 30.4 Ressourcen

Erfasse:

- Neuronenanzahl.
- Synapsenanzahl.
- Anzahl tatsächlich verwendeter Synapsen.
- Speicherbedarf.
- CPU-Zeit.
- Ereignisanzahl.
- Maximale Queuegröße.
- Veränderungsanzahl.
- Ressourcenbedarf pro untersuchter Fähigkeit.
- Zusatzaufwand für Mehrfachverwendung.
- Zusatzaufwand unabhängiger Strukturen.

### 30.5 Keine unbegrenzte Historie

Neuronale Repräsentation darf nicht durch ein externes unbegrenztes Ereignisprotokoll ersetzt werden.

Forschungslogs dienen ausschließlich der Analyse.

---

## 31. Regressionstests

Alle bisherigen **726 Kerntests** müssen erhalten bleiben.

Neue Tests mindestens für:

- Unterschied zwischen momentaner Antwort und erlernter Fähigkeit.
- A-only-Erwerb.
- B-only-Erwerb.
- A → B.
- B → A.
- A → B → A.
- A → B → A+B.
- Gemischtes Lernen.
- Gleiche sensorische Bestandteile in verschiedenen Aufgaben.
- Unterschiedliche zeitliche Reihenfolgen.
- Kontextabhängiger Abruf.
- Gemeinsame neuronale Pfade.
- Kausale Wiederverwendung.
- Vorübergehende Abrufbeeinträchtigung.
- Erleichterter Wiedererwerb.
- Langfristige Interferenz.
- Produktive Reorganisation.
- Isolierte Schreibintervention.
- Erregende und hemmende Konkurrenz.
- R0/R2/S4-Vergleiche.
- Retention.
- Transfer auf C.
- No-Learning-Kontrollen.
- Identische vollständige Zustandskopien.
- Keine Lernwirkung durch diagnostische Prüfungen auf dem Original.
- Geschützte Exploration.
- Persistenz.
- Deterministische Fortsetzung.
- Ressourcenbegrenzung.
- Gültige neuronale Zustände.

Historische Tests dürfen nicht entfernt oder abgeschwächt werden, um die neuen Forschungsannahmen zu unterstützen.

---

## 32. Erfolgskriterien

B.5.11 ist wissenschaftlich erfolgreich, wenn:

1. Die bisherigen Bewertungsannahmen vollständig dokumentiert und kritisch geprüft wurden.
2. Eine reproduzierbare Mehrfachlernaufgabe mit gemeinsamen Informationsbestandteilen vorhanden ist.
3. A und B sowohl sequentiell als auch gemischt untersucht wurden.
4. Die gemeinsame neuronale Nutzung funktional und kausal untersucht wurde.
5. Vorübergehende Abrufbeeinträchtigung von langfristigem Verlust unterschieden wurde.
6. Wiedererwerb und Transfer berücksichtigt wurden.
7. Das Verhältnis zwischen neuronaler Reorganisation und zukünftigem Fähigkeitsumfang untersucht wurde.
8. R0, R2 und S4 unter denselben neuen Aufgaben verglichen wurden.
9. Die Auswirkungen der bisherigen Plastizität auf gemeinsame Repräsentationen untersucht wurden.
10. Der Einfluss begrenzter Netzkapazität ausgewiesen wurde.
11. Ressourceneffizienz und funktionale Redundanz getrennt betrachtet wurden.
12. Unabhängige Bestätigungsdaten vorliegen.
13. Die Grenzen der Ergebnisse ausdrücklich dokumentiert wurden.

### Besonders wichtig

**Ein erfolgreicher Forschungsabschluss setzt nicht voraus, dass Wiederverwendung tatsächlich besser abschneidet.**

Wenn die derzeitige Architektur mehrere Fähigkeiten nicht geeignet integrieren kann, ist das eine wesentliche Erkenntnis.

Ebenso ist es ein gültiges Ergebnis, wenn eine begrenzte Redundanz funktional sinnvoller ist als vollständige Überlappung.

Es darf kein positiver Befund konstruiert werden, um die Ausgangshypothese zu bestätigen.

---

## 33. Keine neue Lernregel in B.5.11

Diese Forschungsstufe dient der Überprüfung unserer Grundlagen.

Daher sind vorerst nicht vorgesehen:

- Neue synaptische Selektionsregeln.
- Automatische Stabilitätsmechanismen.
- Automatische Wiederherstellung früher erfolgreicher Gewichte.
- Technische Repräsentationsetiketten.
- Zwangsweise gemeinsame Speicherung.
- Zwangsweise getrennte Speicherung.
- Künstliche semantische Deduplizierung.
- Neue globale Lernphasen.
- Eine Regel zur Maximierung der bisherigen X/Y-Robustheit.

Falls die Forschung eine konkrete Notwendigkeit für eine neue lokale neuronale Funktion aufzeigt, ist sie zunächst in der Entscheidungsvorlage B.5.12 zu begründen.

Eine Implementierung darf nicht allein aus einer plausiblen Erklärung folgen.

---

## 34. Lieferumfang

Nova-EVE erstellt:

1. Reproduktion B.5.7 bis B.5.10.
2. Kritische Analyse der bisherigen Bewertungsmethoden.
3. Neue Forschungsumgebung für gemeinsame sensorische Bestandteile.
4. Sequentielle A-/B-Lernversuche.
5. Gemischte A-/B-Lernversuche.
6. A → B → A-Wiedererwerb.
7. A → B → A+B-Integration.
8. Transferaufgabe C.
9. Zeitliche Muster- und Kontextprüfung.
10. Vollständige Verhaltensmatrizen.
11. Diagnose gemeinsamer neuronaler Strukturen.
12. Kausale Einzel- und Kombinationsablationen.
13. Untersuchung funktionaler Synapsenwiederverwendung.
14. Untersuchung neuronaler Reorganisation.
15. Neue Definition und Messung von Lerninterferenz.
16. Untersuchung produktiver temporärer Abrufbeeinträchtigungen.
17. R0/R2/S4-Vergleich.
18. Untersuchung vorhandener Eligibility- und Modulationsmechanismen.
19. Prüfung der Grenzen fester Topologien.
20. Ressourceneffizienz- und Redundanzanalyse.
21. Unabhängige Bestätigung.
22. Reproduzierbare Forschungsdaten.
23. Regressionstests.
24. Persistenz- und Determinismusprüfung.
25. Forschungsbericht B.5.11.
26. Technische Analyse neuronaler Repräsentation.
27. Methodische Neubewertung B.5.3 bis B.5.10.
28. Architekturentscheidung.
29. Entscheidungsvorlage B.5.12.
30. Explizite Merge-Empfehlung.

---

## 35. Abschlussfragen

**A.** Wie repräsentiert EVE mehrere erlernte Fähigkeiten innerhalb derselben neuronalen Struktur?

**B.** Werden vorhandene Neuronen und Synapsen beim Erwerb neuer Fähigkeiten tatsächlich wiederverwendet?

**C.** Welche wiederverwendeten Verbindungen sind für mehrere Fähigkeiten kausal notwendig?

**D.** Können bereits bestehende Verbindungen ihre funktionale Bedeutung verändern?

**E.** Ist eine kurzfristig verschlechterte Abrufbarkeit von A während des B-Erwerbs grundsätzlich als Vergessen zu bewerten?

**F.** Können solche Veränderungen notwendige Zwischenschritte zur späteren Fähigkeit A+B sein?

**G.** Wird A nach zwischenzeitlichem B-Erwerb schneller wieder verfügbar als beim ersten Erwerb?

**H.** Erleichtert das Lernen von A und B den Erwerb einer dritten, verwandten Fähigkeit?

**I.** Welche Rolle spielen zeitliche Reihenfolge und Kontext?

**J.** Welche Fähigkeiten lassen sich aus einer einzelnen neuronalen Struktur unter unterschiedlichen Eingangsbedingungen abrufen?

**K.** Welche unserer bisherigen Lern- und Verlustkennzahlen messen vor allem momentane Verhaltensleistung?

**L.** Welche zusätzlichen Kennzahlen sind notwendig, um Lernfortschritt, Fähigkeitserhalt, Transfer und Reorganisation voneinander zu unterscheiden?

**M.** Können R2 oder S4 bestimmte zeitliche Lernprozesse behindern, obwohl sie die bisherige Abrufrobustheit verbessern?

**N.** Welche Rolle spielen Eligibility, positive Modulation und negative Modulation bei gemeinsamer neuronaler Repräsentation?

**O.** Welche Grenzen entstehen durch die bisherige feste Topologie?

**P.** Ist eine Wiederverwendung vorhandener neuronaler Strukturen ressourceneffizienter als getrennte Repräsentationen?

**Q.** Wann ist Redundanz funktional sinnvoll?

**R.** Welche bisherigen Forschungsentscheidungen müssen aufgrund neuer Erkenntnisse anders interpretiert werden?

**S.** Ist unsere derzeitige neuronale Architektur grundsätzlich in der Lage, mehrere zusammenhängende Fähigkeiten in gemeinsamen Strukturen zu integrieren?

**T.** Welche konkrete Forschungsfrage ergibt sich daraus für B.5.12?

---

## 36. Merge-Entscheidung

Der Abschlussbericht muss ausdrücklich enthalten:

**B.5.11 nach `main` mergen: Ja oder Nein.**

### Merge empfohlen, wenn

- Die Forschungsaufgaben reproduzierbar sind.
- Alle Regressionstests bestehen.
- Die bisherigen Bewertungsverfahren erhalten bleiben.
- Neue Bewertungsverfahren nicht in das neuronale Lernen eingreifen.
- Externe Aufgabenbezeichnungen nicht als neuronales Wissen eingeführt werden.
- Die Produktionslogik unverändert bleibt.
- Persistenz und Determinismus erhalten bleiben.
- Keine kritischen technischen Integrationsfehler bestehen.

### Merge nicht empfohlen, wenn

- Die Forschungsumgebung eine gewünschte Repräsentation technisch erzwingt.
- Globale Aufgabeninformationen in neuronale Verbindungen eingeschrieben werden.
- Erfolg allein durch vorgefertigte richtige Antworten entsteht.
- Diagnostische Prüfungen den ursprünglichen Lernverlauf unkontrolliert verändern.
- Historische Regressionen abgeschwächt werden.
- Neue Funktionen ohne Kausalnachweis aktiviert werden.
- Persistenz oder Determinismus fehlschlagen.

Ein negativer wissenschaftlicher Befund ist kein automatischer Merge-Blocker.

Die Empfehlung muss getrennt ausweisen:

- Integration der Forschungsinfrastruktur.
- Wissenschaftliche Ergebnisse.
- Freigabe oder Nichtfreigabe neuer neuronaler Produktionsmechanismen.

---

## 37. Verbindliche Schlussleitlinie

**EVE soll keine neuronale Festplatte werden.**

Eine künstliche neuronale Architektur sollte nicht darauf reduziert werden, jede neue Information in einer weiteren unabhängigen Kopie bisheriger Informationsbestandteile abzulegen.

Wir wollen untersuchen, ob neue Fähigkeiten durch die Veränderung, Wiederverwendung und Kombination vorhandener neuronaler Strukturen entstehen.

Dabei muss berücksichtigt werden, dass dieselben Verbindungen für mehrere Fähigkeiten relevant sein können.

Eine synaptische Veränderung kann eine bestehende Fähigkeit unmittelbar beeinträchtigen und trotzdem für den Erwerb einer zusätzlichen Fähigkeit notwendig sein.

Daraus folgt:

**Nicht jede momentane Verschlechterung ist Vergessen.**

**Nicht jede erhaltene Antwort ist ein Nachweis ausreichender Lernfähigkeit.**

**Nicht jede Gewichtsänderung ist Lernen.**

**Nicht jede unveränderte Synapse ist ein Zeichen für Stabilität.**

Erfolgreiches Lernen muss anhand der tatsächlich entstandenen und unter geeigneten Bedingungen nutzbaren Fähigkeiten bewertet werden.

Die neuronale Architektur darf dabei weder auf unveränderliche Wissensspeicher noch auf die möglichst häufige Veränderung synaptischer Gewichte optimiert werden.

Entscheidend ist, ob EVE durch Erfahrungen ihr funktionales Repertoire entwickeln kann.

**B.5.11 soll deshalb erstmals die gemeinsame neuronale Repräsentation mehrerer Fähigkeiten, ihre zeitliche Verarbeitung und die mögliche produktive Reorganisation bestehender Verbindungen zum eigentlichen Forschungsgegenstand machen.**

Eine überzeugende Erklärung ersetzt keinen kausalen Nachweis.

Eine höhere Erfolgsquote ersetzt keine nachgewiesene Lernfähigkeit.

Und eine vorübergehend schlechtere Antwort ist kein ausreichender Beweis, dass das neuronale System etwas verloren hat.
