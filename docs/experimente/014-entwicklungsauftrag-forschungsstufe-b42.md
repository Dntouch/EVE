# EVE – Forschungsstufe B.4.2
## Erhalt explorativer Handlungsfähigkeit und Aufbau erfolgreicher Verhaltensalternativen

**Auftrag an:** Nova-EVE
**Projekt:** EVE – Emergent Virtual Evolution
**Repository:** `Dntouch/EVE`
**Ausgangsbasis:** `codex/research-stage-b41-learning-validation`
**Implementierung:** C++17
**Status:** Forschungs- und Entwicklungsauftrag

---

# 1. Ausgangssituation

Forschungsstufe B.4.1 hat die methodischen Probleme des ursprünglichen Drei-Faktor-Lernexperiments korrigiert.

Die Ergebnisse bestätigen:

- Simulierte Zeit und neuronale Erholung funktionieren bei korrekter Versuchssteuerung.
- Aufgaben-, Explorations- und Rückmeldungszufall sind getrennt.
- Neuronale Motor- und Modulationskontakte sind funktional getrennt.
- Lokale Lernspuren können synaptische Gewichte verändern.
- Ursprünglich unwirksame Synapsen können durch positive Modulation wirksam werden.
- Eine solche Gewichtsänderung kann nachweislich das spätere motorische Verhalten verändern.

Im kontrollierten Kausalversuch gelang der letzte Nachweis bei allen 64 untersuchten Seeds.

Das zentrale Lernziel wurde dennoch nicht erreicht.

## 1.1 Ergebnisse aus B.4.1

| Variante | Mittlere Belohnung nach Training |
|---|---:|
| K0 – ohne Plastizität | −0,397949 |
| E1 – G3-Lernspur | −0,855957 |
| E2 – prä-/postsynaptische Lernspur | −0,909180 |
| E3 – Membranbeteiligung | −0,977539 |

Alle drei Plastizitätsvarianten verschlechterten das Verhalten gegenüber der Kontrollgruppe.

Nach 512 beziehungsweise 2.048 Trainingserfahrungen erreichten die Lernvarianten eine mittlere Auswertungsbelohnung von −1.

Die neuronale Plastizität führte also nicht zur Auswahl zunehmend erfolgreicher Handlungen, sondern überwiegend zum Verlust funktionierender Handlungswege.

## 1.2 Auffälligkeit bei negativen Rückmeldungen

Im bisherigen Versuchsaufbau existieren explorative Verbindungen, die anfangs stark genug sind, um eine motorische Handlung auszulösen.

Negative Modulation kann diese Verbindungen abschwächen.

Wird beispielsweise eine explorative Verbindung von Gewicht 2 auf 1 reduziert, reicht sie bei einer Feuergrenze von 2 möglicherweise nicht mehr zur Aktivierung des Motorneurons aus.

Dadurch kann folgender Ablauf entstehen:

1. EVE führt explorativ eine Handlung aus.
2. Die Handlung erhält eine negative Bewertung.
3. Die zugehörigen aktiven Verbindungen werden abgeschwächt.
4. Dieselbe explorative Handlung wird schwerer oder unmöglich.
5. Alternative Verbindungen sind noch nicht ausreichend aufgebaut.
6. EVE erzeugt schließlich keine gültige Handlung mehr.

**Diese Erklärung ist bislang eine durch Implementierung und Messergebnisse gestützte Hypothese, aber noch kein vollständig isolierter Kausalnachweis.**

## 1.3 Auffälligkeit bei wiederholtem Einzelreiz

Die bisherige Kontrolle K15 mit identischem wiederholtem Eingangsreiz erreichte eine mittlere Auswertungsbelohnung von +1.

Allerdings lag die frühe Belohnung bereits bei ungefähr +0,797.

Eine direkt passende Kontrollgruppe ohne synaptische Plastizität und mit identischer Eingabefolge fehlte.

Deshalb ist das Ergebnis kein ausreichender Lernnachweis.

Es rechtfertigt jedoch einen gezielten Einzelreizversuch.

---

# 2. Zentrale Forschungsfrage

**Kann EVE durch eigene Erfahrungen eine erfolgreichere Handlung entwickeln, wenn ihre angeborene Fähigkeit zur Exploration auch nach negativen Rückmeldungen erhalten bleibt?**

Dabei sollen drei mögliche Ursachen des bisherigen Scheiterns getrennt untersucht werden:

1. Verlust angeborener explorativer Motorwege durch synaptische Abschwächung.
2. Zu starke oder zu häufige negative Modulation.
3. Unzureichende Exploration nach erfolglosen Handlungen.

Erst danach wird erneut untersucht, ob EVE erfolgreiches Verhalten durch Erfahrung aufbauen kann.

---

# 3. Verbindliche Forschungsentscheidung

B.4.2 führt **keine grundsätzlich neue synaptische Lernregel** ein.

Die bisherigen Drei-Faktor-Mechanismen bleiben erhalten:

- E1 – G3-basierte Lernberechtigung.
- E2 – prä-/postsynaptische Beteiligung.
- E3 – lokale Membranbeteiligung.
- Bestehende positive und negative Modulation.
- Bestehende Gewichtsgrenzen.
- Bestehende lokale Lernspurverwaltung.
- Bestehender ereignisgesteuerter Verfall.

Zulässig sind experimentelle Änderungen an:

- Angeborener explorativer Verschaltung.
- Schutz bestimmter angeborener Funktionen vor Plastizität.
- Häufigkeit und Stärke negativer Modulation.
- Zeitlicher Verteilung explorativer Aktivität.
- Reaktion des angeborenen Explorationssystems auf Rückmeldung.
- Technischem Aufbau und Auswertung der Lernumgebung.

Änderungen am eigentlichen synaptischen Lernmechanismus sind nur nach gesonderter Begründung und als klar ausgewiesene Vergleichsvariante zulässig.

Die bestehende Plastizität bleibt außerhalb der Forschungsversuche standardmäßig deaktiviert.

---

# 4. Grundlegende Architekturprinzipien

EVE bleibt ein ereignisgesteuertes neuronales System.

Weiterhin gelten:

- Kein globaler neuronaler Tick.
- Keine periodischen vollständigen Netzwerkdurchläufe.
- Keine Zeitstempel pro Synapse.
- Keine globale technische Handlungs-ID im neuronalen Zustand.
- Keine technische Auswahl der richtigen Handlung.
- Keine technische Rückwärtsmarkierung beteiligter Synapsen.
- Keine vortrainierten Verhaltenslösungen.
- Keine semantische Bedeutung einzelner Neuronen.
- Keine künstliche Wiederherstellung des Ausgangszustands zwischen Trainingserfahrungen.

## 4.1 Verantwortung des Stammhirns

Das Stammhirn darf angeborene Eigenschaften bereitstellen.

Dazu gehört ausdrücklich die grundlegende Fähigkeit zur Exploration.

Diese Fähigkeit darf unabhängig davon bestehen, welche konkreten Handlungen EVE später als erfolgreich oder erfolglos erlernt.

Das Stammhirn darf jedoch nicht festlegen, welche Handlung bei einem bestimmten Reiz richtig ist.

## 4.2 Verantwortung des neuronalen Netzwerks

Das neuronale Netzwerk muss weiterhin selbst:

- sensorische Reize verarbeiten,
- motorische Aktivität erzeugen,
- lokale Lernspuren bilden,
- modulierende Signale empfangen,
- synaptische Gewichtungen verändern,
- sein späteres Verhalten entsprechend seiner veränderten Struktur beeinflussen.

---

# 5. Forschungsphase I – Reproduktion und Diagnose des Explorationsverlusts

## 5.1 Ziel

Untersuche zuerst, ob die bisherige neuronale Handlungsfähigkeit tatsächlich durch negative synaptische Veränderungen verloren geht.

Die bisherige B.4.1-Konfiguration bleibt dafür unverändert.

## 5.2 Versuchsablauf

Für jeden Seed:

1. Ausgangsnetzwerk initialisieren.
2. Beide motorischen Ausgänge auf Erreichbarkeit prüfen.
3. Eine Trainingssequenz mit korrekter Rückmeldung durchführen.
4. Synaptische Gewichtsänderungen erfassen.
5. Nach definierten Trainingsabschnitten die Erreichbarkeit beider Ausgänge erneut prüfen.
6. Entwicklung der Handlungshäufigkeiten dokumentieren.

## 5.3 Messpunkte

Mindestens nach:

- 0 Erfahrungen.
- 8 Erfahrungen.
- 16 Erfahrungen.
- 32 Erfahrungen.
- 64 Erfahrungen.
- 128 Erfahrungen.
- 512 Erfahrungen.

## 5.4 Diagnostische Informationen

Erfasse:

- Anzahl noch funktionierender explorativer Wege.
- Anzahl abgeschwächter explorativer Synapsen.
- Anzahl vollständig unwirksam gewordener explorativer Synapsen.
- Anzahl neu wirksam gewordener Sensorverbindungen.
- Erreichbarkeit von A.
- Erreichbarkeit von B.
- Anteil erfolgloser motorischer Versuche.
- Anteil vollständig ausbleibender Handlungen.

## 5.5 Kausalitätsprüfung

Erstelle getrennte diagnostische Zustandskopien.

In diesen dürfen ausschließlich für die Forschung:

- bestimmte explorative Synapsen auf ihren ursprünglichen Wert gesetzt werden,
- andere Synapsen unverändert bleiben,
- identische sensorische und explorative Reize wiederholt werden.

Untersuche, ob dadurch zuvor verlorene Handlungswege erneut erreichbar werden.

Diese diagnostischen Eingriffe dürfen nicht in den eigentlichen Trainingsverlauf einfließen.

**Ziel ist der Nachweis, ob die Abschwächung explorativer Verbindungen den beobachteten Handlungsverlust tatsächlich verursacht.**

---

# 6. Forschungsphase II – Angeborene Exploration erhalten

## 6.1 Forschungsfrage

Benötigt EVE angeborene neuronale Verbindungen, welche die grundsätzliche Fähigkeit zum Ausprobieren von Handlungen dauerhaft gewährleisten?

## 6.2 Biologische Orientierung

Ein biologisches Nervensystem muss nicht jede grundlegende motorische Fähigkeit erst durch Belohnung erwerben.

Angeborene Schaltungen stellen grundlegende Reaktions- und Bewegungsmöglichkeiten bereit.

Erfahrung kann anschließend beeinflussen, wann und wie diese Möglichkeiten genutzt werden.

Für EVE untersuchen wir eine vergleichbare Trennung:

**Angeborene Handlungsfähigkeit bleibt erhalten. Erlernte Handlungspräferenzen dürfen sich verändern.**

## 6.3 Versuchsvarianten

### A0 – Vollständig plastische explorative Wege

Unveränderte B.4.1-Referenz.

Explorative Verbindungen können durch positive und negative Modulation verändert werden.

### A1 – Geschützte angeborene Explorationswege

Die grundlegenden explorativen Verbindungen bleiben während des Lernens unverändert.

Die übrigen dafür vorgesehenen Synapsen behalten ihre reguläre Plastizität.

Der Schutz wird als allgemeine Eigenschaft angeborener Verbindungen definiert.

Er darf nicht von der erfolgreichen oder erfolglosen Handlung abhängen.

### A2 – Getrennte explorative und lernfähige Verschaltung

Exploration erreicht die motorischen Ausgänge über eine allgemeine angeborene Verschaltung.

Daneben existieren lernfähige synaptische Wege zwischen sensorischen Eingängen, neuronalen Verarbeitungsbereichen und motorischer Aktivität.

Die beiden Wege dürfen dieselben Motorneuronen beeinflussen.

Die angeborene Exploration legt jedoch keine bevorzugte Handlung fest.

### A3 – Begrenzte angeborene Grundaktivität

Untersuche, ob eine kleine, kontinuierlich verfügbare ereignisgesteuerte Explorationsbereitschaft genügt, ohne feste starke Motorverbindungen vorauszusetzen.

Diese Variante ist optional und nur auszuführen, wenn A1/A2 die Erreichbarkeit nicht ausreichend erklären.

## 6.4 Symmetrie

Für alle angeborenen Varianten gilt:

- A und B müssen gleichwertig erreichbar sein.
- Keine Handlung darf als richtig markiert sein.
- Explorative Ausgangsbedingungen müssen kontrolliert vergleichbar sein.
- Bekannte Unterschiede durch Topologie und Seed sind zu dokumentieren.

Eine geschützte Verbindung darf nicht bedeuten, dass ein bestimmter sensorischer Reiz automatisch die richtige Handlung auslöst.

## 6.5 Schutzmechanismus

Falls für angeborene Synapsen ein nichtplastischer Zustand benötigt wird, muss dieser:

- explizit definiert sein,
- reproduzierbar initialisiert werden,
- in der Persistenz erhalten bleiben,
- von der regulären Gewichtsänderung respektiert werden,
- für Kontrollversuche abschaltbar sein.

Der Schutz darf nicht anhand einer technischen Liste der später erfolgreichen Handlungspfade vergeben werden.

Er ist eine Eigenschaft der angeborenen Funktion, nicht des Lernerfolgs.

## 6.6 Zentrale Messgröße

**Bleiben beide motorischen Handlungsalternativen auch nach zahlreichen negativen Erfahrungen grundsätzlich erreichbar?**

Zusätzlich ist zu prüfen, ob diese Erreichbarkeit allein bereits zu guten Bewertungen führt.

Eine fest erhaltene Handlungsfähigkeit ist noch kein Nachweis für Lernen.

---

# 7. Forschungsphase III – Negative Modulation isolieren

## 7.1 Ziel

Untersuche, welchen Anteil die negative Modulation am Verlust explorativer Handlungsfähigkeit besitzt.

## 7.2 Vergleichsvarianten

### N0 – Keine rückmeldungsabhängige Plastizität

Unveränderte Referenz.

### N1 – Ausschließlich positive Modulation

Gültige lokale Lernspuren können verstärkt werden.

Negative Bewertungen verändern keine synaptischen Gewichte.

### N2 – Ausschließlich negative Modulation

Positive Bewertungen verändern keine Gewichte.

Negative Bewertungen können gültige Synapsen abschwächen.

### N3 – Positive und negative Modulation

Unveränderte Drei-Faktor-Regel aus B.4.1.

### N4 – Reduzierte Häufigkeit negativer Modulation

Untersuche, ob eine geringere Anzahl negativer Modulationsereignisse die Handlungsfähigkeit erhält.

Beispielhafte Vergleichswerte:

- 25 % der negativen Bewertungen bewirken Modulation.
- 50 % bewirken Modulation.
- 75 % bewirken Modulation.
- 100 % bewirken Modulation.

Die Auswahl muss reproduzierbar erfolgen.

Sie darf nicht davon abhängen, welche konkrete Synapse oder Handlung technisch als richtig erkannt wurde.

### N5 – Begrenzte negative Änderungsstärke

Untersuche die bestehende negative Modulation unter unterschiedlichen zulässigen Gewichtsschritten beziehungsweise lokal begrenzten Wirkungen.

Aufgrund ganzzahliger Gewichte muss ausdrücklich beschrieben werden, wie eine reduzierte mittlere Änderungsstärke technisch abgebildet wird.

Keine verdeckten Fließkommagewichte oder nicht dokumentierten Rundungsregeln einführen.

## 7.3 Wichtige Unterscheidung

Eine negative Rückmeldung bedeutet nicht automatisch, dass jede aktive Verbindung für das Ergebnis verantwortlich war.

Die bisherige Drei-Faktor-Lernspur bleibt die lokale Auswahlgrundlage.

Die Modulation darf keine technisch rekonstruierte Fehlerursache direkt auswählen.

## 7.4 Negative Rückmeldung ohne Handlung

Untersuche getrennt:

- Negative Bewertung nach einer tatsächlich ausgeführten, aber erfolglosen Handlung.
- Ausbleibende Bewertung nach keiner Handlung.
- Negative Bewertung trotz fehlender Handlung.
- Neutrale Bewertung bei doppelter Ausgabe.

Insbesondere soll geprüft werden, ob die Bestrafung einer fehlenden Handlung verbliebene explorative Verbindungen weiter zerstört.

Der Vergleich darf nicht durch nachträgliche Auswahl von Synapsen verfälscht werden.

## 7.5 Erfolgskriterium

Eine geringere negative Modulation ist nur dann nützlich, wenn:

1. alternative Handlungen erreichbar bleiben,
2. zugleich eine tatsächliche Verbesserung des zukünftigen Verhaltens möglich wird.

Das bloße Verhindern von Gewichtsänderungen zählt nicht als Lernfortschritt.

---

# 8. Forschungsphase IV – Exploration nach erfolglosen Handlungen

## 8.1 Ausgangsproblem

In B.4.1 findet zusätzliche Exploration nur statt, wenn zuvor weder A noch B ausgelöst wurde.

Führt EVE eine Handlung aus, beendet der Runner die motorische Entscheidung und bewertet das Ergebnis.

Bei einer falschen Handlung wird daher nicht innerhalb desselben Entscheidungsablaufs automatisch eine Alternative erprobt.

## 8.2 Forschungsfrage

**Kann EVE nach einer negativen Erfahrung weiterhin alternative Handlungen ausprobieren, statt lediglich ihre bisherige Handlungsfähigkeit einzubüßen?**

## 8.3 Verbindliche Einschränkung

Die Versuchsumgebung darf EVE nicht mitteilen:

„Du hast A gewählt, also probiere jetzt B.“

Sie darf nur eine allgemeine negative Bewertung zurückmelden.

Die nächste Exploration muss aus den angeborenen neuronalen Mechanismen entstehen.

## 8.4 Vergleichsvarianten

### X0 – Bisherige Exploration

Zusätzliche Exploration findet nur statt, wenn nach dem sensorischen Reiz zunächst keine Handlung entstand.

### X1 – Allgemeines Explorationsfenster

Untersuche eine begrenzte explorative Aktivität nach regulärer sensorischer Verarbeitung, unabhängig davon, ob zuvor A oder B ausgelöst wurde.

Die Aktivität muss durch reguläre neuronale Wege verarbeitet werden.

### X2 – Exploration nach negativer Bewertung

Untersuche eine angeborene, zeitlich begrenzte Erhöhung der allgemeinen Explorationsbereitschaft nach einem negativen modulierenden Signal.

Das Signal darf nur die allgemeine Bereitschaft zum Ausprobieren beeinflussen.

Es darf weder A noch B gezielt auswählen.

Dieser Mechanismus ist als angeborene Regulationsvariante von der synaptischen Drei-Faktor-Regel getrennt zu dokumentieren.

### X3 – Exploration in der folgenden Episode

Eine negative Rückmeldung kann die explorative Aktivität der nächsten regulären Handlungsepisode beeinflussen.

Die sensorische Aufgabenfolge bleibt dabei unverändert.

### X4 – Fortgesetzte Exploration ohne Rückmeldungsabhängigkeit

Eine allgemeine, zeitlich begrenzte Exploration wird auch nach einer motorischen Ausgabe zugelassen.

Diese Kontrolle dient der Trennung zwischen zusätzlicher Exploration und einer spezifischen Reaktion auf negative Rückmeldung.

## 8.5 Zeitliche Zuordnung

Wichtig ist die eindeutige Trennung zwischen:

- Handlung, die gerade bewertet wird.
- Explorationsaktivität nach dieser Bewertung.
- Nächster tatsächlich bewerteter Handlung.

Eine nach der Rückmeldung ausgelöste Handlung darf nicht rückwirkend als Teil der bereits bewerteten Entscheidung gezählt werden.

## 8.6 Mehrere motorische Ausgaben

Wenn innerhalb eines Entscheidungsfensters mehrere Ausgänge aktiv werden, muss eine vorab definierte Regel gelten.

Untersuche insbesondere:

- erste eindeutige Handlung,
- mehrere Ausgaben,
- fehlende Ausgabe,
- weitere Exploration nach Bewertungsabschluss.

Die technische Versuchsumgebung darf keine nachträgliche erfolgreiche Alternative auswählen und die vorherige falsche Entscheidung dadurch verschwinden lassen.

## 8.7 Messgrößen

- Explorative Aktivitäten pro Episode.
- Anteil neu erreichter Handlungsalternativen.
- Anteil wiederholt ausgeführter Handlungen.
- Anteil tatsächlich gewechselter Handlungen.
- Anzahl Erfahrungen zwischen negativer Rückmeldung und nächstem alternativen Versuch.
- Anteil fehlender motorischer Ausgaben.
- Veränderung der synaptischen Gewichtungen.
- Entwicklung der Erfolgsquote.

---

# 9. Forschungsphase V – Zunächst nur ein sensorischer Reiz

## 9.1 Ziel

Untersuche die grundlegende Fähigkeit, eine einzige wiederholte sensorische Situation mit einer erfolgreichen Handlung zu verknüpfen.

Dieser Versuch dient der gezielten Überprüfung des auffälligen K15-Ergebnisses.

## 9.2 Ausgangssituation

EVE erhält bei jeder Episode denselben sensorischen Reiz.

Es stehen weiterhin zwei neuronale motorische Ausgänge zur Verfügung:

- A.
- B.

Die Umgebung bevorzugt zunächst eine der beiden Handlungen.

Die bevorzugte Handlung wird nicht im Netzwerk vorgegeben.

## 9.3 Kontrollierte Zielzuordnung

Um eine systematische Ausgangspräferenz auszuschließen, müssen beide Umweltvarianten untersucht werden:

- A ist erfolgreich.
- B ist erfolgreich.

Beide Varianten werden unter gepaarten Ausgangszuständen verglichen.

Die Auswahl der Umweltregel erfolgt unabhängig von der neuronalen Topologie.

## 9.4 Versuchsgruppen

### S0 – Ohne Plastizität

Gleicher wiederholter Reiz, gleiche Explorationsparameter, aber keine rückmeldungsabhängige Gewichtsänderung.

### S1 – Positive Modulation

Nur positive Erfahrungen verändern gültige lokale Lernspuren.

### S2 – Positive und negative Modulation

Bestehende Drei-Faktor-Regel.

### S3 – Geschützte Exploration mit positiver und negativer Modulation

Angeborene Explorationswege bleiben erhalten.

Lernfähige Synapsen dürfen sich verändern.

### S4 – Zufällige Bewertung

Unabhängige, nicht erfolgsabhängige Modulation.

### S5 – Vertauschte Bewertung

Die Modulationspolarität steht im Gegensatz zur tatsächlichen Umweltbewertung.

### S6 – Keine zusätzliche Exploration

Reguläre sensorische Verarbeitung bleibt aktiv.

Lediglich die explorative Zusatzaktivität wird deaktiviert.

## 9.5 Entscheidende Messgröße

**Entwickelt EVE durch wiederholte Erfahrungen eine stärkere Präferenz für diejenige Handlung, welche die Umgebung positiv bewertet?**

Dabei sind zu erfassen:

- Erfolgsquote zu Beginn.
- Erfolgsquote während des Trainings.
- Erfolgsquote am Ende.
- Erfolgsquote ohne aktive Plastizität.
- Anteil A.
- Anteil B.
- Anteil doppelter Ausgaben.
- Anteil fehlender Ausgaben.

## 9.6 Keine scheinbaren Erfolge

Eine zunächst bereits sehr hohe Erfolgsquote darf nicht als erworbene Fähigkeit ausgegeben werden.

Berichte deshalb die Veränderung gegenüber dem tatsächlichen Ausgangsniveau und gegenüber S0.

Eine Variante gilt nur dann als lernend, wenn ein belastbarer Vorteil gegenüber den passenden Kontrollgruppen entsteht.

---

# 10. Forschungsphase VI – Umkehr der Einzelreizbewertung

## 10.1 Ziel

Untersuche, ob EVE eine erfolgreich erworbene Handlungspräferenz wieder verändern kann.

## 10.2 Versuchsablauf

1. EVE erhält wiederholt denselben sensorischen Reiz.
2. Handlung A wird positiv bewertet.
3. EVE trainiert unter dieser Umweltregel.
4. Die Umgebung ändert ihre Bewertung.
5. Handlung B wird nun positiv bewertet.
6. EVE trainiert weiter, ohne den neuronalen Zustand zurückzusetzen.

Untersuche ebenso die umgekehrte Reihenfolge.

## 10.3 Voraussetzungen

Beide Alternativen müssen auch nach dem ersten Trainingsabschnitt grundsätzlich erreichbar sein.

Ein Netzwerk, das seine gesamte alternative Motorik bereits verloren hat, kann die Umkehr nicht sinnvoll erproben.

Solche Fälle bleiben dennoch Teil der vollständigen Statistik.

## 10.4 Messgrößen

- Präferenz unmittelbar vor der Umkehr.
- Präferenz unmittelbar nach der Umkehr.
- Entwicklung über weitere Erfahrungen.
- Anzahl benötigter Erfahrungen bis zum Wechsel.
- Erreichbarkeit der zuvor weniger bevorzugten Handlung.
- Positive und negative Gewichtsänderungen.
- Anteil vollständiger Handlungsblockaden.

## 10.5 Erfolgskriterium

Eine erfolgreiche Umkehr liegt nur vor, wenn sich die Präferenz aufgrund der veränderten Rückmeldungen tatsächlich in die neue vorteilhafte Richtung verschiebt.

Das einfache Fortbestehen explorativer Aktivität genügt nicht.

---

# 11. Forschungsphase VII – Zwei unterschiedliche sensorische Reize

Erst nach Auswertung des Einzelreizexperiments wird die komplexere Aufgabe erneut untersucht.

## 11.1 Versuchsaufbau

EVE erhält zwei unterscheidbare sensorische Reize.

Die Umwelt bewertet:

- Reiz X → A.
- Reiz Y → B.

Eine zweite Versuchsgruppe erhält die umgekehrte Zuordnung.

## 11.2 Keine vorgegebene Bedeutung

Die neuronale Architektur darf keine besondere Bedeutung von X oder Y kennen.

Die Umweltbewertung bleibt die einzige Quelle für den Erfolg.

## 11.3 Vergleichsgruppen

Verwende die aussagekräftigsten, vorab ausgewählten Varianten aus den vorherigen Forschungsphasen.

Vergleiche weiterhin:

- Keine Plastizität.
- Korrekte Modulation.
- Zufällige Modulation.
- Vertauschte Modulation.
- Geschützte gegen ungeschützte Exploration.
- Unterschiedliche negative Modulationsstärken.
- Unterschiedliche Explorationsbedingungen.

## 11.4 Auswertung

Erfasse nicht nur die allgemeine Erfolgsquote.

Zusätzlich sind getrennt zu messen:

- Wahrscheinlichkeit von A bei X.
- Wahrscheinlichkeit von B bei X.
- Wahrscheinlichkeit von A bei Y.
- Wahrscheinlichkeit von B bei Y.

Damit wird sichtbar, ob EVE tatsächlich reizabhängige Unterschiede entwickelt oder lediglich allgemein einen Motorweg bevorzugt.

---

# 12. Forschungsphase VIII – Positive und negative Modulation getrennt bewerten

## 12.1 Grundproblem

Wenn positive und negative Modulation gemeinsam aktiviert sind, kann eine negative Gewichtsänderung den Aufbau erfolgreicher Verbindungen überlagern.

Um das zu prüfen, werden die beiden Wirkungen unabhängig untersucht.

## 12.2 Positives Lernen

Prüfe, ob wiederholte positive Erfahrungen dazu führen, dass zuvor unwirksame Verbindungen stärker werden.

Erfasse:

- Nullgewicht-zu-eins-Veränderungen.
- Weitere positive Gewichtsänderungen.
- Neu erreichbare Handlungen.
- Sättigung.
- Gleichzeitige Aktivierung beider Motorwege.

## 12.3 Negatives Lernen

Prüfe, ob negative Erfahrungen tatsächlich zur Abschwächung unvorteilhafter gelernter Verbindungen führen.

Erfasse getrennt:

- Angeborene Explorationsverbindungen.
- Lernfähige sensorische Verbindungen.
- Sonstige Verarbeitungspfade.

## 12.4 Wechselwirkung

Untersuche, ob positive und negative Modulation gemeinsam bessere Ergebnisse liefern als positive Modulation allein.

Eine zusätzliche negative Rückmeldung ist nur dann hilfreich, wenn sie die Verhaltensanpassung verbessert und die notwendigen Handlungsalternativen nicht dauerhaft zerstört.

---

# 13. Forschungsphase IX – Fortlaufender Betrieb

## 13.1 Trainingsumfang

Untersuche mindestens:

- 32 Erfahrungen.
- 128 Erfahrungen.
- 512 Erfahrungen.
- 2.048 Erfahrungen.

## 13.2 Neuronale Zustände

Während des Trainings bleiben erhalten:

- Synaptische Gewichte.
- Vorhandene neuronale Verbindungen.
- Reguläre Membranzustände.
- R2F-Erholungszustände.
- G3-Ereignisgedächtnisse.
- Gültige lokale Lernspuren.
- Reguläre neuronale Aktivitätszustände.

Kurzlebige Zustände dürfen nur aufgrund ihrer vorgesehenen Regeln verfallen.

## 13.3 Zeit

Nutze die in B.4.1 korrigierte explizite Simulationszeit.

Wähle feste, dokumentierte Episodenabstände.

Alle gepaarten Kontrollgruppen erhalten identische äußere zeitliche Bedingungen.

## 13.4 Lern- und Auswertungsphase

Nach dem Training folgt eine Auswertung mit deaktivierter Gewichtsänderung.

Die neuronalen Gewichte bleiben erhalten.

Die Bewertungssignale sollen, soweit erforderlich, weiterhin über vergleichbare neuronale Wege übertragen werden, damit nicht eine veränderte sensorische Umgebung den Vergleich verfälscht.

---

# 14. Forschungsphase X – Statistische Auswertung

## 14.1 Stichprobe

Verwende mindestens 64 reproduzierbare Seeds.

Die Seed-Liste ist vor der abschließenden Auswertung festzulegen.

## 14.2 Gepaarte Versuche

Für jeden Seed müssen die Vergleichsvarianten identische äußere Aufgabenfolgen erhalten.

Die Zufallsquellen bleiben getrennt für:

- Neuronale Initialisierung.
- Explorationsaktivität.
- Sensorische Eingaben.
- Zufällige Rückmeldung.
- Sonstige notwendige Versuchseffekte.

## 14.3 Primäre Zielgröße

Primär ist die mittlere Bewertung während einer unabhängigen Auswertungsphase ohne neue Gewichtsänderungen.

## 14.4 Ergänzende Zielgrößen

- Anteil erfolgreicher Handlungen.
- Anteil fehlender Handlungen.
- Anteil doppelter Ausgaben.
- Erreichbarkeit beider Motorwege.
- Stärke und Richtung der Handlungspräferenz.
- Zahl erlernter wirksamer Verbindungen.
- Anpassungsfähigkeit nach Umweltumkehr.
- Stabilität bei längeren Lernfolgen.

## 14.5 Statistische Verfahren

Berichte:

- Mittelwert.
- Median.
- Streuung über Seeds.
- Gepaarte Unterschiede.
- 95-%-Bootstrapintervalle.
- Anteil verbesserter Seeds.
- Entwicklung im Trainingsverlauf.

Nicht nur erfolgreiche Seeds ausweisen.

## 14.6 Vorab festgelegte Vergleichsbedingungen

Vor der abschließenden Hauptmatrix sind festzulegen:

- Primäre Erfolgskriterien.
- Zentrale Varianten.
- Umfang der Trainings- und Auswertungsphasen.
- Maß für erhaltene Handlungsfähigkeit.
- Regeln für fehlende und doppelte Ausgaben.
- Statistisches Auswertungsverfahren.

Parameter, die anhand einer Voruntersuchung ausgewählt wurden, müssen anschließend auf unabhängigen Seeds oder einem getrennten Bestätigungsdatensatz geprüft werden.

---

# 15. Verbindliche Kontrollen

| Kontrolle | Zweck |
|---|---|
| K0 | Keine synaptische Plastizität |
| K1 | B.4.1-Referenz ohne Explorationsschutz |
| K2 | Geschützte angeborene Exploration |
| K3 | Positive Modulation allein |
| K4 | Negative Modulation allein |
| K5 | Positive und negative Modulation |
| K6 | Reduzierte negative Modulationshäufigkeit |
| K7 | Exploration nach allgemeinem Zeitplan |
| K8 | Exploration nach negativer Rückmeldung |
| K9 | Exploration in der nächsten Episode |
| K10 | Rückmeldung ohne vorausgegangene Handlung |
| K11 | Lernspur ohne Modulation |
| K12 | Modulation ohne gültige Lernspur |
| K13 | Zufällige Modulation |
| K14 | Vertauschte Modulation |
| K15 | Einzelreiz ohne Plastizität |
| K16 | Einzelreiz mit Plastizität |
| K17 | Einzelreiz mit umgekehrter Zielhandlung |
| K18 | Zwei unterschiedliche Reize |
| K19 | Umweltumkehr |
| K20 | Nullgewicht-Synapsen |
| K21 | Erreichbarkeit nach negativen Erfahrungen |
| K22 | Fortgesetztes Training ohne Zustandswiederherstellung |
| K23 | Modulierende Signalroute ohne Gewichtswirkung |
| K24 | Geschützte Verschaltung mit deaktivierter Schutzfunktion |

Die Kontrollen müssen tatsächlich ausgeführt werden.

Keine vordefinierten Ergebniszeilen ohne zugrunde liegende neuronale Versuche.

---

# 16. Sicherheits- und Stabilitätsanforderungen

Die früheren Probleme neuronaler Überaktivität und synaptischer Sättigung bleiben relevant.

## 16.1 Gewichtsbeschränkungen

- Keine negativen Synapsengewichte.
- Keine Überschreitung des Maximalgewichts.
- Einhaltung vorhandener Eingangsgewichtbudgets.
- Keine automatische synaptische Löschung.
- Kein unbegrenztes Synapsenwachstum.
- Keine direkte technische Auswahl von Lernkandidaten.

## 16.2 Exploration

- Keine unkontrollierten Explorationskaskaden.
- Keine unbegrenzte Wiederholung von Motorereignissen.
- Keine direkte Auswahl der erfolgreichen Handlung durch das Stammhirn.
- Keine Veränderung der Umweltbewertung durch neuronale Lernregeln.

## 16.3 Ereignisverwaltung

- Kein globaler Tick.
- Begrenzte Ereignisverarbeitung.
- Deterministische Reihenfolge.
- Keine künstliche Vorverarbeitung zukünftiger Ereignisse.
- Kein Rücksetzen langfristiger neuronaler Zustände.

## 16.4 Abbrüche

Technische Abbrüche müssen protokolliert werden.

Sie dürfen nicht aus der Auswertung entfernt werden.

---

# 17. Ressourcenverbrauch

B.4.2 soll weiterhin den sparsamen Ansatz des gesamten EVE-Projekts verfolgen.

## 17.1 Messgrößen

Erfasse:

- Speicherbedarf je Neuron.
- Speicherbedarf je Synapse.
- Zusätzlichen Zustand geschützter Verbindungen.
- Zusätzliche Explorationsereignisse.
- Zusätzliche modulierende Ereignisse.
- Laufzeitkosten je Lernentscheidung.
- Kosten wiederholter explorativer Aktivität.
- Ereignisse pro Sekunde.
- CPU-Zeit je Episode.
- Maximale Ereigniswarteschlange.
- RAM-Spitzenverbrauch.

## 17.2 Netzgrößen

Untersuche zunächst die etablierte isolierte 64-Neuronen-Schaltung.

Wiederhole die entscheidenden Versuche auf der 256-Neuronen-Konfiguration mit regulärer Vernetzung.

Erstelle zusätzlich klar gekennzeichnete Speicherabschätzungen für:

- 10.000 Neuronen.
- 100.000 Neuronen.
- 1.000.000 Neuronen.

Gemessene Werte und Hochrechnungen müssen getrennt ausgewiesen werden.

---

# 18. Persistenz

Die bestehende Persistenz aus B.4.1 bleibt erhalten.

Zusätzliche angeborene Eigenschaften müssen gegebenenfalls mitgespeichert werden.

Zu prüfen sind:

1. Angeborener Schutz einzelner Verbindungen.
2. Synaptische Gewichte nach mehreren Lernerfahrungen.
3. Gültige lokale Lernspuren.
4. Ausstehende R2F-Erholungen.
5. Ausstehende Modulationen.
6. Aktive explorative Zustände.
7. Ausstehende Ablaufereignisse.
8. Reproduzierbarkeit nach Laden.
9. Fortsetzung der Umweltumkehr.
10. Deterministische Fortsetzung der getrennten Zufallsströme.

Alte Zustandsformate müssen soweit technisch vorgesehen lesbar bleiben.

---

# 19. Automatisierte Tests

Ergänze mindestens folgende Prüfungen.

## Angeborene Exploration

1. A und B sind initial symmetrisch erreichbar.
2. Geschützte Verbindungen behalten ihre Gewichtung.
3. Ungeschützte Verbindungen bleiben plastisch.
4. Schutz hängt nicht von der erfolgreichen Handlung ab.
5. Schutz ist abschaltbar.
6. Schutz wird korrekt persistiert.
7. Angeborene Exploration führt nicht automatisch zur richtigen Handlung.
8. Motorische Exploration entsteht über neuronale Ereignisse.

## Negative Modulation

9. Negative Modulation schwächt zulässige Verbindungen.
10. Geschützte explorative Verbindungen bleiben erhalten.
11. Positive Modulation bleibt unverändert möglich.
12. Reduzierte negative Modulationshäufigkeit ist reproduzierbar.
13. Keine negative Änderung ohne gültige Lernspur.
14. Negative Modulation ohne Handlung wird gemäß Kontrollvariante behandelt.
15. Eingangsbudget und Gewichtsgrenzen bleiben erhalten.

## Exploration nach Rückmeldung

16. Negative Bewertung kann allgemeine Exploration beeinflussen, wenn die Variante aktiviert ist.
17. Die Rückmeldung wählt keinen bestimmten Motorweg.
18. Nachfolgende Exploration wird nicht rückwirkend zur vorherigen Handlung gezählt.
19. Eine weitere Handlung entsteht nur durch neuronale Aktivität.
20. Motorische Doppelausgaben werden korrekt erfasst.
21. Fehlende Ausgaben bleiben Teil der Statistik.
22. Keine unbegrenzten Explorationsschleifen.

## Einzelreizexperiment

23. Kontrollgruppe ohne Lernen erhält identische Eingaben.
24. A und B können beide Umweltzielhandlungen sein.
25. Kein Zielwissen wird neuronal vorgegeben.
26. Ausgangspräferenzen werden dokumentiert.
27. Erlernte Präferenzen bleiben ohne aktive Plastizität überprüfbar.
28. Zufällige und vertauschte Rückmeldungen bleiben reproduzierbar.
29. Umweltumkehr verändert keine neuronale Konfiguration direkt.
30. Kein künstlicher Reset zwischen Trainingsepisoden.

## Zwei-Reiz-Experiment

31. Beide Reize werden unabhängig neuronal verarbeitet.
32. Reizabhängige motorische Ausgaben sind getrennt auswertbar.
33. Eine allgemeine Motorpräferenz wird nicht als reizabhängiges Lernen ausgegeben.
34. Gleiche Seeds erhalten gleiche externe Aufgabenfolgen.

## Regression und Persistenz

35. B.4.1-Referenz bleibt reproduzierbar.
36. R2F funktioniert.
37. G3 bleibt erhalten.
38. Eligibility bleibt korrekt.
39. Modulatorische Rezeptorkontakte bleiben nicht erregend.
40. Speichern und Laden geschützter Verbindungen.
41. Fortsetzen eines laufenden Lernversuchs.
42. Keine doppelten Modulationswirkungen.
43. Alle bisherigen 260 automatisierten Tests bestehen weiterhin.

---

# 20. Forschungsbericht

Erstelle einen vollständigen Forschungsbericht mit:

## A. Ausgangslage

B.4.1-Ergebnisse und konkrete Hypothesen.

## B. Kausaldiagnose

Führt negative Modulation tatsächlich zum Verlust explorativer Motorwege?

## C. Angeborene Exploration

Vergleich der Varianten A0 bis A3.

## D. Negative Modulation

Vergleich N0 bis N5.

## E. Exploration nach Rückmeldung

Vergleich X0 bis X4.

## F. Einzelreiz-Lernen

Prüfung der früheren K15-Auffälligkeit mit passenden Kontrollgruppen.

## G. Umweltumkehr

Kann eine einmal aufgebaute Präferenz durch Erfahrung wieder verändert werden?

## H. Zwei-Reiz-Lernen

Entsteht reizabhängiges Verhalten?

## I. Lokalität der Plastizität

Welche Synapsen verändern sich und welche motorischen Folgen entstehen daraus?

## J. Stabilität

Synaptische Sättigung, neuronale Überaktivität und Verlust motorischer Erreichbarkeit.

## K. Ressourcen

RAM, CPU, Ereignisse und Skalierung.

## L. Abschlussbewertung

Wurde erfahrungsabhängiges Lernen nachgewiesen?

Falls nicht: Welche konkret untersuchte Ursache begrenzt es weiterhin?

---

# 21. Lieferumfang

Erstelle im Repository:

1. Forschungsauftrag und technische Dokumentation B.4.2.
2. Reproduzierbare Diagnose des bisherigen Explorationsverlusts.
3. Implementierungen der Varianten A0 bis A3, soweit für die Versuche erforderlich.
4. Vergleich der negativen Modulationsvarianten.
5. Vergleich der Explorationsvarianten nach Rückmeldung.
6. Kontrolliertes Einzelreizexperiment mit passender No-Learning-Kontrolle.
7. Umweltumkehr beim Einzelreiz.
8. Zwei-Reiz-Experiment.
9. Maschinenlesbare Rohdaten.
10. Gepaarte statistische Auswertung über mindestens 64 Seeds.
11. Dokumentierte motorische Erreichbarkeit.
12. Gewichtsverlaufsdaten.
13. Kausale Ablationskontrollen.
14. Ressourcen- und Stabilitätsmessungen.
15. Persistenzprüfungen.
16. Erweiterte automatisierte Tests.
17. Vollständigen Forschungsbericht.
18. Entscheidungsvorlage für die nächste Forschungsstufe.

Vorhandene Forschungsdaten bleiben unverändert.

Fremde Arbeitsbaumänderungen dürfen nicht unbeabsichtigt verändert oder committed werden.

---

# 22. Verbindliche Forschungsgrenzen

Noch nicht implementieren:

- Neue allgemeine synaptische Lernregeln.
- Neue hemmende Synapsentypen.
- STDP mit individuellen synaptischen Zeitstempeln.
- Automatische Entfernung von Synapsen.
- Globale technische Handlungserkennung als Lernhilfe.
- Vorgegebene korrekte Handlungsfolgen.
- Semantische Wortrepräsentationen.
- Sprachentwicklung.
- Produktive Chatto-Kommunikation.
- Eigenständige Erwartungsbildung.
- Vorhersagefehler als neue Lernregel.

B.4.2 untersucht ausschließlich den Erhalt neuronaler Exploration und ihre Wechselwirkung mit dem bestehenden Drei-Faktor-Lernmechanismus.

---

# 23. Abschlusskriterien

Am Ende müssen folgende Fragen empirisch beantwortet sein:

1. Werden explorative Motorwege durch negative Modulation tatsächlich unwirksam?
2. Kann dieser Verlust durch kausale Ablation beziehungsweise Wiederherstellung einzelner Gewichte nachgewiesen werden?
3. Können angeborene explorative Wege dauerhaft erhalten bleiben?
4. Bleiben A und B dabei symmetrisch und ohne vorgegebene Lösung erreichbar?
5. Kann EVE trotz geschützter Exploration weiterhin synaptisch lernen?
6. Welche Wirkung hat positive Modulation allein?
7. Welche Wirkung hat negative Modulation allein?
8. Welche Wirkung hat eine geringere negative Modulationshäufigkeit?
9. Hilft fortgesetzte Exploration nach einer erfolglosen Handlung?
10. Kann EVE bei einem einzigen wiederholten Reiz eine erfolgreiche Handlungspräferenz entwickeln?
11. Ist die Verbesserung gegenüber einer identischen Kontrollgruppe ohne Plastizität nachweisbar?
12. Kann EVE eine erworbene Präferenz nach Umkehr der Bewertung verändern?
13. Können zwei sensorische Reize unterschiedliche erlernte Handlungen auslösen?
14. Bleiben alternative Motorwege auch bei langen Trainingsfolgen erreichbar?
15. Wie stark beeinflussen die zusätzlichen Mechanismen den Ressourcenverbrauch?
16. Welcher konkrete Mechanismus begrenzt das Lernen weiterhin, falls keine Verbesserung entsteht?

Ein positives Ergebnis darf nicht ausschließlich aus erhaltener Motoraktivität bestehen.

**Die entscheidende Frage bleibt, ob EVE durch Erfahrung häufiger die vorteilhafte Handlung ausführt.**

---

# 24. Übergeordnetes Forschungsziel

Das langfristige EVE-Konzept orientiert sich an biologischen Grundprinzipien.

Dabei unterscheiden wir:

**Angeborene Fähigkeiten**

Grundlegende Voraussetzungen für Wahrnehmung, neuronale Aktivität, Exploration und Handlungsfähigkeit.

**Erworbene Fähigkeiten**

Veränderungen neuronaler Verbindungen und Verhaltensweisen durch eigene Erfahrungen.

Bisher kann EVE durch synaptische Plastizität bereits ihre eigenen Handlungsmöglichkeiten verändern.

Allerdings zerstört die negative Modulation möglicherweise genau die angeborenen Möglichkeiten, auf deren Grundlage neue Erfahrungen entstehen könnten.

B.4.2 soll untersuchen, ob die dauerhafte Verfügbarkeit einer grundlegenden Explorationsfähigkeit dieses Problem beseitigen kann.

---

## Zentraler Forschungsgrundsatz

**Eine künstliche Intelligenz kann nur aus Alternativen lernen, wenn sie diese Alternativen auch weiterhin ausprobieren kann.**

Negative Erfahrungen dürfen erlernte Verbindungen verändern.

Sie dürfen aber nicht zwangsläufig die grundlegende angeborene Fähigkeit zerstören, neue Handlungswege zu erkunden.

Das Stammhirn soll Möglichkeiten bereitstellen.

Die neuronalen Verbindungen sollen durch Erfahrungen entscheiden lernen, welche Möglichkeiten vorteilhaft sind.

**Ziel von B.4.2 ist nicht, EVE die richtige Handlung beizubringen. Ziel ist, ihr die Fähigkeit zum Lernen zu erhalten, während sie eigene Erfahrungen sammelt.**
