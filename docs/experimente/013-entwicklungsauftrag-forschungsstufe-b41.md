# EVE – Forschungsstufe B.4.1
## Validierung des erfahrungsabhängigen Lernversuchs
### Zeitverhalten, Exploration, neuronale Entscheidungen und überprüfbare Lernwirkung

**Auftrag an:** Nova-EVE
**Projekt:** EVE – Emergent Virtual Evolution
**Repository:** `Dntouch/EVE`
**Ausgangsbasis:** `codex/research-stage-b4-three-factor-plasticity`
**Status:** Forschungs- und Validierungsauftrag
**Implementierung:** C++17

---

# 1. Ausgangssituation

Forschungsstufe B.4 führte erstmals einen experimentellen Mechanismus für rückmeldungsabhängige synaptische Plastizität ein.

Dieser Mechanismus orientiert sich am biologischen Drei-Faktor-Lernen:

1. Präsynaptische Aktivität.
2. Postsynaptische Aktivität beziehungsweise lokale Beteiligung.
3. Ein später eintreffendes modulierendes Signal.

Die ersten beiden Faktoren können eine lokale, zeitlich begrenzte Lernspur erzeugen.

Ein modulierendes Signal kann anschließend eine synaptische Gewichtsänderung bewirken.

## 1.1 Ergebnisse aus B.4

Die technische Machbarkeit wurde nachgewiesen.

Unter E2 wurden über 64 Seeds beobachtet:

- 512 positive Gewichtsänderungen.
- 256 negative Gewichtsänderungen.
- Keine neu wirksamen Nullgewicht-Synapsen.

Ein belastbarer Verhaltensvorteil gegenüber der Kontrollgruppe ohne Plastizität entstand jedoch nicht.

| Variante | Mittlere Auswertungsbelohnung |
|---|---:|
| K0 – keine Plastizität | 0,026367 |
| E2 – prä-/postsynaptische Lernspur | 0,026367 |
| E1 – G3-Kandidaten | −0,150391 |
| E3 – Membranbeteiligung | −0,539551 |

Auch die Umkehr der Umweltbewertung führte nicht zum gewünschten Anpassungsverhalten.

Die bisherige Schlussfolgerung lautet daher:

**Synaptische Plastizität ist technisch möglich, aber erfahrungsabhängiges Lernen wurde nicht nachgewiesen.**

## 1.2 Methodische Probleme

Eine anschließende Codeprüfung identifizierte mehrere Einschränkungen des Versuchsaufbaus.

### Problem A – Fehlender regulärer Zeitfortschritt

Im Hauptversuchsrunner wird die simulierte Zeit zwischen regulären Trainingsepisoden nicht ausdrücklich fortgeschaltet.

Die neuen Lernspuren erhalten jedoch Ablaufzeitpunkte relativ zur simulierten Zeit.

Dadurch können eigentlich kurzlebige Lernspuren über zahlreiche Trainingsepisoden bestehen bleiben, sofern sie nicht anderweitig verbraucht oder überschrieben werden.

Die beabsichtigte zeitliche Lernberechtigung wird damit nicht korrekt untersucht.

### Problem B – Stark vereinfachtes neuronales Netzwerk

B.4 verwendet eine eigene kleine Versuchsschaltung:

- 64 Neuronen.
- Keine regulären zufälligen Ausgangssynapsen.
- Feuergrenze 1.
- Keine explizit aktivierte R2F-Erholung.
- Vorgegebene experimentelle Verbindungen zwischen Eingängen und zwei motorischen Ausgängen.

Damit untersuchte B.4 zwar einen isolierten Plastizitätsmechanismus, jedoch nicht die vollständige bisherige neuronale EVE-Konfiguration.

### Problem C – Gekoppelte Zufallsfolgen

Die zufällige Modulation verwendet im B.4-Versuchsrunner denselben Zufallszahlengenerator wie die Auswahl zukünftiger Eingangsreize.

Zusätzliche Zufallsziehungen für Rückmeldungen können somit die spätere Aufgabenfolge verändern.

Kontrollgruppen erhalten dadurch nicht zwangsläufig dieselben externen Aufgabenfolgen.

### Problem D – Fehlende unabhängige Exploration

Die bisherige Kontrollvariante ohne Exploration unterdrückt zugleich die sensorische Aktivierung.

Damit wird nicht ausschließlich Exploration entfernt.

Für eine gültige Kontrolle müssen reguläre Eingaben weiterhin stattfinden.

### Problem E – Unklare Wirkung der Gewichtsänderungen

Unter E2 wurden Gewichte verändert, aber keine neuen Nullgewicht-Synapsen aktiviert und keine Verhaltensverbesserung erreicht.

Es ist noch nicht ausreichend untersucht, ob die geänderten Verbindungen überhaupt die Wahrscheinlichkeit zukünftiger motorischer Entscheidungen beeinflussen.

---

# 2. Zentrale Forschungsfrage

**Kann EVE mit dem bereits implementierten Drei-Faktor-Mechanismus unter methodisch korrekten Bedingungen aus wiederholten eigenen Erfahrungen lernen?**

Dabei sind drei Fragen unabhängig voneinander zu beantworten:

1. Funktioniert die neuronale Lernregel technisch korrekt unter realistischer Ereigniszeit?
2. Verändern die synaptischen Anpassungen tatsächlich die Erreichbarkeit und Auswahl motorischer Handlungen?
3. Führt die resultierende Veränderung zu einer nachweisbaren Verbesserung gegenüber geeigneten Kontrollgruppen?

Eine positive Antwort auf Frage 1 reicht nicht für Frage 3.

Ebenso beweist eine fehlende Verbesserung noch nicht, dass das zugrunde liegende Drei-Faktor-Prinzip ungeeignet ist.

---

# 3. Verbindliche Forschungsentscheidung

B.4.1 ist eine **Validierung und methodische Korrektur**, keine freie Neuentwicklung des neuronalen Systems.

Deshalb gilt:

- Bestehende Drei-Faktor-Plastizität bleibt Ausgangsbasis.
- Bestehende Eligibility- und Modulationsvarianten bleiben vergleichbar.
- Keine neuen semantischen Fähigkeiten.
- Keine globale technische Handlungsidentifikation.
- Keine technische Synapsenauswahl nach bekannter Lösung.
- Keine globale neuronale Uhr mit periodischen Netzwerkscans.
- Keine Zeitstempel pro Synapse.
- Keine neue vollständige Netzwerkarchitektur ohne experimentelle Begründung.

Notwendige Korrekturen an Versuchsrunner, Ereignisplanung und technischen Fehlern sind zulässig.

**Jede Änderung an der eigentlichen neuronalen Lernregel muss gesondert gekennzeichnet und gegen die bisherige Regel getestet werden.**

Vorhandene B.4-Ergebnisse dürfen nicht nachträglich überschrieben werden.

Neue Rohdaten erhalten eigene B.4.1-Dateien.

---

# 4. Forschungsphase I – Reproduktion und Code-Audit

## 4.1 Ziel

Die bisherigen B.4-Ergebnisse müssen zuerst reproduzierbar bestätigt werden.

## 4.2 Ausgangsreproduktion

Führe den bisherigen Versuchsrunner ohne inhaltliche Änderungen erneut aus.

Vergleiche:

- Anzahl Trainingsepisoden.
- Anzahl Auswertungsepisoden.
- Belohnungsverteilungen.
- Positive Gewichtsänderungen.
- Negative Gewichtsänderungen.
- Nullgewicht-zu-eins-Änderungen.
- Anzahl erfolgreicher und erfolgloser Seeds.
- Persistenz und Regressionstests.

Die ursprünglichen Ergebnisse müssen rekonstruierbar bleiben.

## 4.3 Zeitdiagnose

Instrumentiere den bisherigen Runner so, dass für jede Episode mindestens folgende Werte verfügbar sind:

- Simulationszeit bei Episodenbeginn.
- Simulationszeit nach sensorischer Aktivierung.
- Simulationszeit beim motorischen Ausgang.
- Simulationszeit der Rückmeldung.
- Simulationszeit bei Episodenende.
- Anzahl aktiver Lernspuren.
- Anzahl abgelaufener Lernspuren.
- Anzahl überschriebener Lernspuren.
- Anzahl verbrauchter Lernspuren.
- Anzahl fälliger, aber noch nicht verarbeiteter Ereignisse.

Prüfe ausdrücklich, ob die bisherige Hauptmatrix ihre Simulationszeit tatsächlich fortschaltet.

Eine aus dem Quellcode abgeleitete Vermutung ist im Bericht von einer experimentell bestätigten Beobachtung zu unterscheiden.

## 4.4 Zusätzliche Codeprüfung

Untersuche außerdem:

- Werden zukünftige Erholungsereignisse korrekt zurückgehalten?
- Werden zukünftige Eligibility-Abläufe korrekt zurückgehalten?
- Werden Rückmeldungen zeitlich richtig verarbeitet?
- Können Lernspuren über mehrere Episoden unbeabsichtigt aktiv bleiben?
- Werden modulierte Änderungen möglicherweise mehrfach ausgeführt?
- Können Rückmeldungsimpulse motorische Ausgänge beeinflussen?
- Sind die beiden Aktionsneuronen strukturell unabhängig von der Bewertungsroute?
- Beeinflussen unterschiedliche Versuchsvarianten unbeabsichtigt dieselben Zufallsströme?

Die Befunde müssen vor der Korrektur dokumentiert werden.

---

# 5. Forschungsphase II – Korrekte simulierte Zeit

## 5.1 Ziel

Jede Trainingsepisode erhält eine definierte zeitliche Position innerhalb des fortlaufenden neuronalen Systems.

Eine Episode darf nicht deshalb zeitlos sein, weil der technische Runner sofort die nächste Aufgabe startet.

## 5.2 Grundprinzip

Verwende die bestehende ereignisgesteuerte Simulationszeit.

Zeitfortschritt erfolgt weiterhin ausschließlich über die dafür vorgesehenen Laufzeitfunktionen.

Nicht zulässig sind:

- Ein globaler neuronaler Tick.
- Periodisches Aktualisieren aller Synapsen.
- Automatisches Abarbeiten beliebiger zukünftiger Ereignisse beim Antwortende.
- Zurücksetzen der Simulationszeit zwischen Trainingsepisoden.

## 5.3 Verbindliche Episodenfolge

Eine Episode muss zeitlich nachvollziehbar ablaufen:

1. Sensorischer Reiz wird bereitgestellt.
2. Reguläre neuronale Verarbeitung findet statt.
3. Motorische Aktivität wird beobachtet.
4. Das Entscheidungsfenster endet.
5. Die Umwelt bestimmt anhand der beobachteten Ausgabe ihre Bewertung.
6. Die Rückmeldung wird zum vorgesehenen Simulationszeitpunkt bereitgestellt.
7. Die Rückmeldung wird neuronal verarbeitet.
8. Fällige Erholungs- und Ablaufereignisse werden gemäß ihrer tatsächlichen Zeitplanung verarbeitet.
9. Die simulierte Zeit erreicht den Beginn der nächsten Episode.

Die Reihenfolge bei gleichen Zeitpunkten muss deterministisch sein.

## 5.4 Zeitabstände

Vergleiche mindestens:

- Kein Abstand zwischen Episoden als bewusst definierte Extremkontrolle.
- Abstand 4.
- Abstand 16.
- Abstand 32.
- Abstand 64 Simulationseinheiten.

Die konkreten Werte sind abstrakte Vergleichsparameter.

Sie dürfen nicht stillschweigend mit realen Millisekunden gleichgesetzt werden.

## 5.5 Lernspurendauer

Untersuche:

- Dauer 4.
- Dauer 16.
- Dauer 64.

Berichte getrennt, wie viele Lernspuren:

- innerhalb derselben Episode verarbeitet werden,
- zur folgenden Episode noch gültig sind,
- aufgrund natürlichen Zeitablaufs verschwinden,
- durch weitere neuronale Aktivität überschrieben werden,
- durch Modulation verbraucht werden.

## 5.6 Zeitliche Kontrollfragen

- Verfällt eine Spur ohne neue neuronale Eingänge?
- Wird eine genau zum Ablaufzeitpunkt eintreffende Rückmeldung korrekt behandelt?
- Können Ablaufereignisse nach einem Speichern und Laden korrekt fortgesetzt werden?
- Ändert die Episodendauer die Lernergebnisse?
- Kommen vermeintliche Lernerfolge möglicherweise durch künstlich lange erhaltene Markierungen zustande?

---

# 6. Forschungsphase III – Trennung sämtlicher Zufallsquellen

## 6.1 Ziel

Die verschiedenen Kontrollgruppen müssen unter denselben äußeren Bedingungen verglichen werden.

Unterschiedliche Modulationsarten dürfen nicht unbeabsichtigt unterschiedliche Aufgaben erzeugen.

## 6.2 Getrennte Zufallsströme

Verwende logisch unabhängige deterministische Zufallsquellen für:

1. Initiale neuronale Topologie.
2. Angeborene Grundverschaltungen.
3. Sensorische Aufgabenfolge.
4. Exploration.
5. Zufällige Umwelt- beziehungsweise Kontrollrückmeldung.
6. Gegebenenfalls separate stochastische Versuchseffekte.

Die Seeds müssen dokumentiert sein.

## 6.3 Gemeinsame Aufgabenfolgen

Für einen gegebenen Ausgangsseed müssen alle gepaarten Varianten dieselbe vorab definierte sensorische Aufgabenfolge erhalten.

Das gilt insbesondere für:

- K0 ohne Lernen.
- K2 mit korrekter Modulation.
- K3 mit zufälliger Modulation.
- K4 mit vertauschter Modulation.
- E1, E2 und E3.

Zufällige Bewertungen dürfen nicht den nächsten Eingangsreiz verändern.

Die tatsächlichen neuronalen Ereignisfolgen dürfen sich hingegen infolge geänderter Gewichte unterscheiden.

Solche Unterschiede sind Teil des untersuchten Lernprozesses.

## 6.4 Exploration

Die Exploration erhält ebenfalls einen eigenständigen reproduzierbaren Zufallsstrom.

Die Umgebung darf nicht einfach eine zufällige motorische Handlung erzeugen.

Explorative Reize oder intrinsische neuronale Aktivierungen müssen über das reguläre neuronale System wirksam werden.

Die resultierende Motorausgabe entsteht ausschließlich aus tatsächlicher neuronaler Aktivität.

## 6.5 Reproduzierbarkeit

Prüfe:

- Gleicher Seed und gleiche Variante erzeugen identische Ergebnisse.
- Zufällige Bewertung verändert nicht die äußere Aufgabenfolge.
- Veränderungen der Modulation verändern nicht die Initialtopologie.
- Kontrollvarianten unterscheiden sich nur durch die vorgesehenen Versuchsparameter.

---

# 7. Forschungsphase IV – Überprüfung der Zwei-Handlungs-Umgebung

## 7.1 Ziel

Stelle sicher, dass die Lernumgebung tatsächlich eine neuronale Entscheidung zwischen zwei möglichen Ausgängen untersucht.

## 7.2 Handlungen

Behalte zunächst zwei getrennte motorische Ergebnisse bei:

- A.
- B.

Das tatsächliche Ergebnis wird ausschließlich anhand neuronaler Aktivität bestimmt.

Dokumentiere:

- Nur A.
- Nur B.
- Beide.
- Keine Handlung.

Doppelte und fehlende Ausgaben dürfen nicht verworfen werden.

## 7.3 Strukturelle Erreichbarkeit

Prüfe für beide Handlungswege:

- Anzahl eingehender Synapsen.
- Anfangsgewichtungen.
- Summe wirksamer Eingangsgewichtungen.
- Erreichbarkeit vom sensorischen Eingang.
- Reaktion auf explorative Aktivität.
- Veränderungen während des Trainings.

Ein bevorzugter Ausgang darf nicht bereits durch eine asymmetrische Startverdrahtung systematisch vorgegeben sein.

## 7.4 Funktionale Erreichbarkeit

Untersuche beide Ausgänge vor Beginn des eigentlichen Lernens.

Dabei muss festgestellt werden, ob A und B unter vergleichbaren Bedingungen tatsächlich durch neuronale Aktivität ausgelöst werden können.

Wenn einzelne Seeds nur einen der beiden Ausgänge erreichen, muss dies im Bericht sichtbar bleiben.

Es sind zwei Auswertungen zulässig:

**Vollständige Stichprobe:** Alle Seeds einschließlich unzureichender Exploration.

**Diagnostische Teilstichprobe:** Seeds, bei denen beide Handlungen vor dem Lernen tatsächlich erreichbar sind.

Die Teilstichprobe darf nicht nach späterem Lernerfolg ausgewählt werden.

## 7.5 Trennung der neuronalen Rollen

Prüfe besonders die bisherige Verwendung von Zielneuronen der Bewertungsroute als motorische Ausgänge.

Sensorische Eingänge, motorische Ausgänge und modulierende Rückmeldungswege müssen funktional unterscheidbar sein.

Eine Rückmeldung darf nicht deshalb als Handlungsentscheidung erscheinen, weil sie unmittelbar das Motorneuron aktiviert.

Untersuchungen mit vorhandener gemeinsamer Verschaltung sind als eigene Kontrolle zulässig.

---

# 8. Forschungsphase V – Tatsächliche neuronale Exploration

## 8.1 Problem

Die bisherige K12-Kontrolle unterdrückt die sensorische Aktivierung.

Dadurch wird nicht allein Exploration entfernt.

## 8.2 Neue Kontrollregel

Die Umgebung liefert bei sämtlichen Vergleichsvarianten dieselben sensorischen Eingaben.

Unterschiede entstehen ausschließlich durch die jeweils untersuchte Exploration.

## 8.3 Varianten

### X0 – Keine zusätzliche Exploration

Normale sensorische Verarbeitung ohne explorative Zusatzaktivität.

### X1 – Geringe Exploration

Begrenzte explorative Feuerereignisse in dafür zulässigen neuronalen Verarbeitungsbereichen.

### X2 – Mittlere Exploration

Höhere, aber weiterhin begrenzte Explorationsaktivität.

### X3 – Etablierter E4-neu-Ansatz

Verwende die bereits bekannte E4-neu-Exploration als Vergleich.

Direkte Aktivierung eines motorischen Ausgangs durch den technischen Zufallsgenerator gilt nicht als neuronale Entscheidung.

## 8.4 Vergleichsmaße

- Häufigkeit von A und B.
- Anteil keiner Handlung.
- Anteil doppelter Ausgaben.
- Zahl tatsächlich explorativ aktivierter Neuronen.
- Zusätzliche neuronale Ereignisse.
- Entstehende Lernspuren.
- Veränderung der erreichbaren Handlungswege.

## 8.5 Entscheidende Frage

**Kann EVE alternative Handlungen weiterhin ausprobieren, nachdem eine bereits verwendete Verbindung durch negative Modulation abgeschwächt wurde?**

Das ist insbesondere für spätere Umkehrversuche wichtig.

---

# 9. Forschungsphase VI – Isolierte Überprüfung von E1, E2 und E3

## 9.1 Ziel

Die bisherigen Lernspurvarianten werden unter korrekter simulierter Zeit erneut untersucht.

## 9.2 Varianten

### E0 – Keine Plastizität

Unveränderte Referenz.

### E1 – G3-Kandidaten

Markierung der zuletzt relevanten Eingangssynapsen gemäß bestehender Regel.

### E2 – Prä-/postsynaptische Feuerbeteiligung

Markierung anhand präsynaptischer Aktivität und einer tatsächlichen postsynaptischen Feuerentscheidung.

### E3 – Lokale Membranbeteiligung

Markierung auch bei lokaler synaptischer Beteiligung ohne notwendiges Feuern.

## 9.3 Zu untersuchende Eigenschaften

Für jede Variante:

- Anzahl erzeugter Lernspuren.
- Anzahl gültiger Lernspuren bei Rückmeldung.
- Anzahl verbrauchter Lernspuren.
- Anzahl abgelaufener Lernspuren.
- Zahl positiver Gewichtsänderungen.
- Zahl negativer Gewichtsänderungen.
- Zahl erstmals wirksam werdender Nullgewicht-Synapsen.
- Betroffene neuronale Bereiche.
- Einfluss auf A/B-Motorwege.
- Einfluss auf nachfolgende Entscheidungen.

## 9.4 Keine Vermischung

E1, E2 und E3 müssen zunächst mit identischer Modulationsverteilung und identischer Umwelt verglichen werden.

Erst anschließend dürfen zusätzliche Parameter kombiniert werden.

---

# 10. Forschungsphase VII – Nullgewicht-Synapsen

## 10.1 Ausgangsproblem

E2 veränderte im bisherigen B.4-Versuch Gewichte, aktivierte aber keine Verbindung von Gewicht 0 auf Gewicht 1.

Das ist für die ursprüngliche EVE-Architektur relevant.

Neue Synapsen beginnen grundsätzlich mit Gewicht 0.

## 10.2 Ziel

Untersuche, warum E2 keine zuvor unwirksamen Verbindungen aktiviert.

Mögliche Ursachen sind:

- Die Lernspur entsteht nur am feuerentscheidenden Eingang.
- Ein Nullgewicht-Eingang löst selbst keine Feuerentscheidung aus.
- Die Modulation erreicht nicht die betreffenden Neuronen.
- Die Lernspur wird vor Eintreffen der Rückmeldung entfernt.
- Die Kandidatenkapazität reicht nicht aus.
- Andere Eingänge verdrängen die Nullgewicht-Synapsen.
- Gewichtsbudgets oder andere Grenzen verhindern die Änderung.

Diese Hypothesen sind experimentell zu trennen.

## 10.3 Kontrollierter Einzelversuch

Erzeuge eine kleine lokale Verschaltung mit:

- Mindestens einem bereits wirksamen Eingang.
- Mindestens einem zusätzlichen Eingang mit Gewicht 0.
- Tatsächlicher Aktivierung beider Eingänge.
- Einer möglichen postsynaptischen Feuerentscheidung.
- Einer späteren modulierenden Rückmeldung.

Untersuche, unter welchen Bedingungen die Nullgewicht-Synapse eine gültige Lernspur erhält und erstmals Gewicht 1 erreicht.

Die Auswahl darf nicht aufgrund einer technisch bekannten richtigen Handlung erfolgen.

## 10.4 Abgrenzung

Eine Nullgewicht-Synapse kann durch Aktivität lernberechtigt sein, obwohl sie noch keinen positiven Beitrag zum Membranpotenzial leistet.

Diese Möglichkeit ist für den Aufbau neuer wirksamer Verbindungen ausdrücklich zu erhalten.

Neue Lernregeln sind nur dann zulässig, wenn die bestehende Regel zunächst vollständig untersucht wurde.

---

# 11. Forschungsphase VIII – Zusammenhang zwischen Gewicht und Verhalten

## 11.1 Ziel

Prüfe, ob die tatsächlichen synaptischen Gewichtsänderungen die nachfolgende Handlungsentscheidung beeinflussen können.

## 11.2 Diagnose

Für jede relevante Änderung erfasse:

- Synapsen-ID.
- Senderneuron.
- Empfängerneuron.
- Gewicht vor Änderung.
- Gewicht nach Änderung.
- Art der Modulation.
- Gültige lokale Lernspur.
- Zeitpunkt der Änderung.
- Strukturellen Zusammenhang mit motorischen Ausgängen.

Die letzten Zusammenhänge werden ausschließlich durch die unabhängige Forschungsdiagnose geprüft.

Die neuronale Lernregel darf diese Informationen nicht verwenden.

## 11.3 Kontrollierte Wiederholung

Prüfe nach einer beobachteten Gewichtsänderung dieselben sensorischen Eingaben erneut.

Vergleiche:

- Aktivitätsverteilung vor der Änderung.
- Aktivitätsverteilung nach der Änderung.
- Erreichbarkeit beider motorischen Ausgänge.
- Wahrscheinlichkeit beziehungsweise Häufigkeit einer erfolgreichen Ausgabe.

## 11.4 Zusätzliche Ablationskontrolle

Eine gesonderte diagnostische Kopie eines Zustands darf testweise ausgewählte Gewichte auf den vorherigen Zustand zurücksetzen.

Das dient nur der Prüfung, ob diese Gewichtsänderung ursächlich zum veränderten Verhalten beiträgt.

Diese Untersuchung darf nicht in den fortgeführten Trainingslauf eingreifen.

**Ein Trainingserfolg muss ohne solche technische Eingriffe entstehen.**

---

# 12. Forschungsphase IX – Korrigierter Hauptlernversuch

## 12.1 Ausgangslage

Der bisherige Hauptversuch verwendete:

- 128 Trainingserfahrungen.
- 64 Auswertungserfahrungen.
- 64 Seeds.

Diese Vergleichsbasis soll zunächst beibehalten werden.

## 12.2 Primärer Versuchsaufbau

Untersuche:

1. K0 – Keine synaptische Plastizität.
2. E1 – G3-basierte Plastizität.
3. E2 – Prä-/postsynaptische Plastizität.
4. E3 – Membranbasierte Plastizität.
5. K3 – Zufällige Modulation.
6. K4 – Vertauschte Modulation.
7. K5 – Verzögerte Modulation.
8. Kontrollgruppe ohne zusätzliche Exploration.

Alle Varianten erhalten dieselben vorab festgelegten äußeren Aufgabenfolgen.

## 12.3 Zeitliche Bedingungen

Der Hauptvergleich muss unter identischen definierten Episodenabständen stattfinden.

Zusätzlich werden die Einflüsse unterschiedlicher Abstände separat untersucht.

## 12.4 Trainingszustand

Während des Trainings bleiben sämtliche regulären neuronalen Zustände erhalten.

Keine Wiederherstellung des Ausgangszustands zwischen Erfahrungen.

## 12.5 Auswertung

Nach Abschluss des Trainings erfolgt eine separate Phase mit:

- Erhaltenen erlernten Synapsengewichten.
- Deaktivierter neuer Gewichtsänderung.
- Weiterhin regulärer neuronaler Verarbeitung.
- Identischer externer Aufgabenverteilung.

Das bloße Ausbleiben von Bewertungen darf nicht unbemerkt den gesamten neuronalen Ablauf in einer Weise verändern, die den Vergleich verfälscht.

Gegebenenfalls ist zwischen einer Bewertung ohne plastische Wirkung und vollständig ausbleibender sensorischer Rückmeldung zu unterscheiden.

## 12.6 Erweiterte Versuche

Wenn die ursprüngliche Matrix technisch valide und stabil läuft, untersuche zusätzlich:

- 512 Trainingserfahrungen.
- 2.048 Trainingserfahrungen.

Die Erweiterung darf nicht nur für zuvor erfolgreiche Varianten durchgeführt werden.

---

# 13. Forschungsphase X – Echte Umweltumkehr

## 13.1 Ziel

Untersuche, ob EVE eine erworbene Handlungspräferenz wieder verändern kann.

## 13.2 Ablauf

Trainiere zunächst unter einer festen Umweltregel.

Beispiel:

- Bei Reiz X ist A vorteilhaft.
- Bei Reiz Y ist B vorteilhaft.

Nach einem festgelegten Abschnitt werden die Umweltbedingungen umgekehrt.

Anschließend gilt:

- Bei Reiz X ist B vorteilhaft.
- Bei Reiz Y ist A vorteilhaft.

Die neue Zuordnung darf nicht neuronal vorgegeben werden.

## 13.3 Kontrollen

Vergleiche:

- Keine Plastizität.
- Korrekte Modulation.
- Zufällige Modulation.
- Umgekehrte Modulation.
- Unterschiedliche Explorationsstärken.
- Unterschiedliche Lernspurenvarianten.

## 13.4 Auswertung

Messe:

- Erfolgsquote vor der Umkehr.
- Erfolgsquote unmittelbar nach der Umkehr.
- Erfolgsentwicklung nach weiteren Erfahrungen.
- Wiedererreichbarkeit zuvor unterdrückter Handlungswege.
- Zahl negativer Gewichtsänderungen.
- Zahl neu verstärkter alternativer Verbindungen.
- Eventuelle Sättigung und dauerhafte Blockade.

Ein Netzwerk, das sich nicht umstellen kann, darf nicht als erfolgreich adaptiv bewertet werden.

---

# 14. Forschungsphase XI – Vergleich mit regulärer EVE-Architektur

## 14.1 Problem

Der ursprüngliche B.4-Versuch verwendet lediglich 64 Neuronen mit einer speziell vorbereiteten Ausgangsschaltung.

Es ist noch offen, ob dieselben Ergebnisse mit der etablierten EVE-Architektur auftreten.

## 14.2 Zwei getrennte Untersuchungen

### A – Isolierte Lernschaltung

Behalte zunächst die kleine 64-Neuronen-Umgebung.

Ihr Zweck ist die methodisch kontrollierte Untersuchung einzelner Lernmechanismen.

### B – Bestehende EVE-Konfiguration

Untersuche anschließend eine Konfiguration mit:

- 256 Neuronen.
- 1.024 regulären Startsynapsen.
- Etablierter räumlicher Vernetzung.
- V2 als angeborener Grundverschaltung.
- R2F-Erholung.
- G3-Ereignisgedächtnis.
- E4-neu-Exploration.
- Zwei neuronalen motorischen Ausgängen.
- Drei-Faktor-Plastizität.

Die motorische Zwei-Ausgangs-Aufgabe darf zusätzliche angeborene technische Kontaktpunkte besitzen.

Sie darf aber keine vorgegebene richtige Handlung enthalten.

## 14.3 Vergleich

Untersuche:

- Unmittelbare motorische Erreichbarkeit.
- Explorationsfähigkeit.
- Lokale Lernspuren.
- Erreichbarkeit modulierender Signale.
- Tatsächliche Gewichtsänderungen.
- Stabilität.
- Wiederholte Handlungen.
- Verhaltensänderung durch Erfahrung.

Wenn die größere Konfiguration nicht lernen kann, ist zu unterscheiden, ob der Lernmechanismus oder bereits die motorische Erreichbarkeit beziehungsweise die Exploration scheitert.

---

# 15. Forschungsphase XII – Methodisch saubere Kontrollgruppen

Mindestens folgende Kontrollen sind erforderlich.

| Kontrolle | Zweck |
|---|---|
| K0 | Keine Plastizität |
| K1 | Alte unmodulierte Verstärkung |
| K2 | Korrekte Rückmeldung |
| K3 | Zufällige Rückmeldung mit unabhängigem RNG |
| K4 | Vertauschte Rückmeldung bei identischer Aufgabenfolge |
| K5 | Verzögerte Rückmeldung |
| K6 | Modulation ohne gültige Spur |
| K7 | Lernspur ohne Modulation |
| K8 | Rückmeldung ohne beobachtete Handlung |
| K9 | Rückmeldungsinduzierte Spuren |
| K10 | Nullgewicht-Synapsen |
| K11 | Eingangsgewichtbudgets |
| K12 | Sensorische Eingaben ohne zusätzliche Exploration |
| K13 | Identische Ausgangszustände |
| K14 | Ausbleibende Rückmeldung |
| K15 | Wiederholte sensorische Eingaben |
| K16 | Keine Zeitfortschaltung als Altverhaltenskontrolle |
| K17 | Korrekte Zeitfortschaltung |
| K18 | Unterschiedliche Episodenabstände |
| K19 | Unterschiedliche Lernspurendauern |
| K20 | 64-Neuronen- gegen 256-Neuronen-Konfiguration |
| K21 | Getrennte motorische und modulierende Kontaktbereiche |
| K22 | Training ohne Gewichtsänderung bei erhaltener sensorischer Bewertung |

Alle Kontrollen müssen tatsächlich ausgeführt werden.

Statische Ergebnisbehauptungen ohne zugrunde liegende Läufe sind nicht zulässig.

---

# 16. Statistische Bewertung

## 16.1 Reproduzierbarkeit

Verwende mindestens 64 gepaarte Seeds.

Die Seed-Liste wird vor dem abschließenden Vergleich festgelegt.

## 16.2 Primäre Zielgröße

Primär ist die mittlere Belohnung in der Auswertungsphase unter deaktivierter Plastizität.

Fehlende und doppelte Ausgaben werden nach einer vorab festgelegten Regel berücksichtigt.

## 16.3 Sekundäre Größen

- Anteil korrekt ausgeführter Handlungen.
- Anteil A.
- Anteil B.
- Anteil beider Ausgänge.
- Anteil keiner Handlung.
- Änderung gegenüber der frühen Trainingsphase.
- Lernfortschritt über die Episoden.
- Anzahl erkennbar adaptiver Seeds.
- Erfolge bei Umweltumkehr.
- Änderungen der Synapsengewichte.
- Verfügbarkeit alternativer motorischer Wege.

## 16.4 Gepaarter Vergleich

Berechne insbesondere:

- E2 gegen K0.
- E2 gegen zufällige Modulation.
- E2 gegen vertauschte Modulation.
- E1 gegen E2.
- E2 gegen E3.

Berichte gepaarte Differenzen je Seed.

Verwende Unsicherheitsintervalle, vorzugsweise durch Bootstrap über die unabhängigen Seeds.

Die endgültigen Erfolgskriterien sind vor der Hauptauswertung festzulegen.

Ein Lernerfolg wird nur dann als nachgewiesen bewertet, wenn ein belastbarer Verhaltensvorteil gegenüber der passenden Kontrollgruppe vorliegt.

## 16.5 Keine Ergebnisoptimierung durch Seed-Auswahl

Die vollständige Seed-Stichprobe ist immer auszuweisen.

Eine zusätzliche Analyse funktional explorationsfähiger Netzwerke ist zulässig.

Sie darf jedoch nicht die vollständige Auswertung ersetzen.

Parameter dürfen nicht anhand der abschließenden Testdaten ausgewählt und anschließend auf denselben Daten als unabhängig bestätigt ausgegeben werden.

---

# 17. Stabilität und synaptische Regulation

## 17.1 Frühere Erfahrungen

Vor B.4 hatten die ersten neuronalen Lernversuche gezeigt, dass unkontrollierte Gewichtsverstärkung zu übermäßiger Aktivität führen kann.

Diese Erkenntnisse bleiben verbindlich.

## 17.2 Messgrößen

- Ereignisse pro Episode.
- Feuerereignisse pro Episode.
- Synaptische Impulse pro Episode.
- Aktive und unwirksame Synapsen.
- Gewichtungsverteilung.
- Eingangsgewichte je Neuron.
- Refraktär unterdrückte Feuerentscheidungen.
- Neuronale Erholungsereignisse.
- Überschriebene Lernspuren.
- Abgelaufene Lernspuren.
- Modulationsereignisse.
- Technische Abbrüche.
- Langfristige Erreichbarkeit motorischer Ausgänge.

## 17.3 Vergleich

Untersuche:

- Unterschiedliche Gewichtsobergrenzen.
- Unterschiedliche Eingangsbudgets.
- Positive Modulation.
- Negative Modulation.
- Kombinierte Modulation.
- Unterschiedliche Lernspurendauern.
- Unterschiedliche Explorationsstärken.

Neue Regulationsmechanismen dürfen nur nach eigenständiger Begründung hinzugefügt werden.

---

# 18. Ressourcenverbrauch

B.4.1 muss weiterhin zum sparsamen lokalen EVE-Konzept passen.

## 18.1 Anforderungen

- Ereignisgesteuerte Verarbeitung.
- Kein globaler neuronaler Tick.
- Keine unnötigen Netzwerkscans.
- Begrenzte lokale Speicher.
- Keine GPU-Pflicht.
- Reproduzierbare Verarbeitung.
- Keine unbegrenzt wachsende Lernhistorie.

## 18.2 Messgrößen

Erfasse:

- Speicherbedarf pro Neuron.
- Speicherbedarf pro Synapse.
- Speicherbedarf pro Lernspur.
- Zusätzliche Ereignisse je Trainingserfahrung.
- Ereignisse pro Sekunde.
- CPU-Zeit pro Episode.
- Kosten der Modulation.
- Kosten des zeitlichen Verfalls.
- Maximale Ereigniswarteschlange.
- Spitzen-RAM-Bedarf.

## 18.3 Skalierung

Vergleiche tatsächlich gemessene Werte für die untersuchten kleinen Netzwerke.

Erstelle zusätzlich klar gekennzeichnete Hochrechnungen für:

- 10.000 Neuronen.
- 100.000 Neuronen.
- 1.000.000 Neuronen.

Die bisherige lineare Abschätzung von 702,25 Byte pro Neuron darf nicht ungeprüft als allgemeiner Speicherbedarf zukünftiger EVE-Netzwerke übernommen werden.

Insbesondere sind feste Laufzeitkosten, Neuronenspeicher, Synapsenspeicher und aktive Ereigniswarteschlangen getrennt zu betrachten.

---

# 19. Persistenz

Die bereits vorhandene Format-9-Persistenz bleibt Ausgangsbasis.

Zu prüfen sind:

1. Speichern nach einer neuronalen Gewichtsänderung.
2. Speichern mit gültigen Lernspuren.
3. Speichern mit ausstehenden Lernspurabläufen.
4. Speichern mit ausstehenden Rückmeldungen.
5. Speichern während einer längeren Trainingssequenz.
6. Fortsetzen nach Wiederherstellung.
7. Identische externe Aufgabenfolge nach Wiederherstellung.
8. Identischer neuronaler Zustand bei identischem weiteren Ereignisablauf.
9. Keine erneute Ausführung bereits verbrauchter Modulationen.
10. Keine Wiederherstellung abgelaufener Lernspuren.
11. Erhalt der erlernten Gewichte.
12. Abwärtskompatibilität früherer Zustandsformate.

Auch die getrennten Zufallszustände der Versuchsumgebung sind bei reproduzierbaren Fortsetzungen zu berücksichtigen.

---

# 20. Automatisierte Tests

Ergänze die bisherige Testsuite um mindestens folgende Prüfungen.

## Simulationszeit

1. Zeit schreitet zwischen regulären Episoden vorwärts.
2. K0 erhält dieselben äußeren Zeitabstände wie lernende Varianten.
3. Lernspuren verfallen zum vorgesehenen Zeitpunkt.
4. Fällige Ereignisse werden vollständig verarbeitet.
5. Zukünftige Ereignisse werden nicht vorzeitig verarbeitet.
6. Ablauf ohne weitere Eingänge.
7. Rückmeldung unmittelbar vor Ablauf.
8. Rückmeldung exakt beim Ablauf.
9. Rückmeldung nach Ablauf.
10. Wiederherstellung mit zukünftigen Ereignissen.

## Zufallsquellen

11. Aufgabenfolge unabhängig von zufälliger Modulation.
12. Initialtopologie unabhängig von Umweltbewertung.
13. Exploration unabhängig von Bewertungssampling.
14. Gepaarte Kontrollvarianten erhalten identische äußere Aufgabenfolgen.
15. Gleicher Seed erzeugt identische Ergebnisse.

## Motorische Handlungen

16. Handlung A neuronal erreichbar.
17. Handlung B neuronal erreichbar.
18. Doppelte Ausgabe korrekt erkannt.
19. Fehlende Ausgabe korrekt erkannt.
20. Explorationskontrolle erhält sensorische Eingaben.
21. Keine technische Auswahl einer Handlung.
22. Rückmeldung wird nicht versehentlich als reguläre Handlung gezählt.
23. Getrennte motorische und modulierende Kontakte.

## Plastizität

24. E1 erhält bisherige Funktion.
25. E2 erhält bisherige Funktion.
26. E3 erhält bisherige Funktion.
27. Nullgewicht-Synapse kann lokal lernberechtigt werden.
28. Positive Modulation kann Gewicht 0 auf 1 erhöhen.
29. Negative Modulation kann Gewicht reduzieren.
30. Gewichtsgrenzen bleiben erhalten.
31. Eingangsbudgets bleiben erhalten.
32. Abgelaufene Spuren erzeugen keine Änderung.
33. Rückmeldungsinduzierte Spuren bleiben ausgeschlossen.
34. Keine doppelte Modulationswirkung.
35. Ausbleibende Modulation bewirkt keine neue rückmeldungsabhängige Änderung.

## Verhalten

36. Training ohne Lernen bleibt Kontrollreferenz.
37. Belohnungsumkehr verändert ausschließlich die Umweltbewertung.
38. Training und unabhängige Auswertung bleiben getrennt.
39. Erlernte Gewichte bleiben während der Auswertung erhalten.
40. Keine künstliche Wiederherstellung zwischen Trainingserfahrungen.
41. Tatsächlich neuronale Handlungsentscheidungen werden ausgewertet.
42. Erfolgsmetriken erfassen alle Ausgabekategorien.
43. Keine nachträgliche Entfernung erfolgloser Seeds.
44. Gleiche Datengrundlage für gepaarte Vergleiche.
45. Abbrüche werden als solche dokumentiert.

## Regression

Alle bisherigen 215 automatisierten Tests müssen weiterhin erfolgreich bleiben.

---

# 21. Forschungsbericht

Erstelle einen vollständigen Bericht mit folgenden Abschnitten.

## A. Ausgangssituation

Wesentliche Ergebnisse und Einschränkungen aus B.4.

## B. Reproduktionsprüfung

Sind die ursprünglichen B.4-Rohdaten erneut reproduzierbar?

## C. Code-Audit

Bestätigte methodische Probleme und tatsächliche technische Ursachen.

## D. Simulierte Zeit

Vergleich bisheriger und korrigierter Zeitplanung.

## E. Zufallsquellen

Nachweis identischer externer Aufgabenfolgen bei gepaarten Versuchen.

## F. Neuronale Zwei-Handlungs-Umgebung

Strukturelle und funktionale Erreichbarkeit.

## G. Exploration

Vergleich der Explorationsvarianten.

## H. Lernspuren

E1, E2 und E3 unter korrektem Zeitfortschritt.

## I. Nullgewicht-Synapsen

Ursachen für ausbleibende Neugewichtung.

## J. Gewichtsänderung und Handlungswirkung

Nachweis des Zusammenhangs zwischen Synapsenänderung und späterer Entscheidung.

## K. Hauptlernexperiment

Ergebnisse aus 128 Trainingserfahrungen und 64 Auswertungserfahrungen.

## L. Erweiterte Lernverläufe

512 und 2.048 Erfahrungen, soweit technisch sinnvoll.

## M. Umweltumkehr

Untersuchung der Anpassungsfähigkeit.

## N. Vergleich 64 gegen 256 Neuronen

Übertragbarkeit auf die bestehende EVE-Architektur.

## O. Ressourcen und Stabilität

Speicher, CPU, Ereignisaufkommen und neuronale Regulation.

## P. Abschlussbewertung

Wurde tatsächliches erfahrungsabhängiges Lernen nachgewiesen?

Falls nicht: Welcher konkrete Mechanismus begrenzt den Lernfortschritt?

---

# 22. Lieferumfang

Erstelle im Repository:

1. Entwicklungs- und Forschungsdokumentation B.4.1.
2. Audit des bisherigen B.4-Versuchsaufbaus.
3. Reproduktion der bestehenden B.4-Ergebnisse.
4. Korrigierten Versuchsrunner mit explizitem Zeitfortschritt.
5. Getrennte deterministische Zufallsquellen.
6. Validierte neuronale Zwei-Handlungs-Umgebung.
7. Echte Explorationskontrollen.
8. Vergleich E1, E2 und E3.
9. Gesonderte Nullgewicht-Synapsen-Untersuchung.
10. Messung des Einflusses von Gewichtsänderungen auf motorische Entscheidungen.
11. Gepaarte Hauptversuche über mindestens 64 Seeds.
12. Kontrollversuche mit zufälliger und vertauschter Modulation.
13. Untersuchung der Umweltumkehr.
14. Vergleich mit der bestehenden 256-Neuronen-Konfiguration.
15. Ressourcen- und Stabilitätsdaten.
16. Persistenztests.
17. Erweiterte automatisierte Regressionstests.
18. Vollständigen Forschungsbericht.
19. Eine Entscheidungsvorlage zur weiteren Lernentwicklung.

Rohdaten müssen maschinenlesbar sein.

Dokumentiere insbesondere die vollständigen zeitlichen Abläufe und die getrennten Zufallsquellen.

Bestehende B.4-Rohdaten bleiben unverändert.

Bereits vorhandene fremde Arbeitsbaumänderungen dürfen nicht ungefragt verändert oder committed werden.

---

# 23. Abschlusskriterien

B.4.1 ist erst abgeschlossen, wenn folgende Fragen empirisch beantwortet sind:

1. War die Simulationszeit in B.4 tatsächlich fehlerhaft beziehungsweise unzureichend fortgeschaltet?
2. Welche Auswirkungen hatte das auf die Lebensdauer der Lernspuren?
3. Können die bisherigen B.4-Ergebnisse exakt reproduziert werden?
4. Erhalten gepaarte Kontrollgruppen nach der Korrektur identische äußere Aufgabenfolgen?
5. Sind beide Handlungen tatsächlich neuronal erreichbar?
6. Gibt es eine echte, unabhängig kontrollierbare Exploration?
7. Bleibt R2F über wiederholte Handlungen funktionsfähig?
8. Können E1, E2 und E3 bei korrektem Zeitfortschritt Gewichte verändern?
9. Können Nullgewicht-Synapsen durch Erfahrung wirksam werden?
10. Welche Gewichtsänderungen beeinflussen tatsächlich spätere motorische Entscheidungen?
11. Verbessert sich EVE gegenüber der Kontrollgruppe ohne Lernen?
12. Ist ein möglicher Vorteil gegenüber zufälliger Modulation nachweisbar?
13. Kann EVE nach veränderter Umweltbewertung ihre Präferenz anpassen?
14. Welche Ergebnisse lassen sich auf die 256-Neuronen-Konfiguration übertragen?
15. Bleiben Speicherbedarf und Rechenleistung im vorgesehenen Rahmen?
16. Welche konkrete neuronale Eigenschaft müsste für einen weiteren Lernversuch verändert werden?

Ein negativer Lernerfolg nach methodischer Validierung ist ein zulässiges Forschungsergebnis.

Unzulässig wäre lediglich, ein methodisch ungeprüftes Experiment erneut als grundsätzlichen Nachweis für oder gegen neuronales Lernen zu interpretieren.

---

# 24. Übergeordnete Forschungsentscheidung

Vor der nächsten Architekturänderung soll eindeutig geklärt werden:

**Scheitert das Lernen, weil die neuronale Plastizitätsregel ungeeignet ist – oder weil unser bisheriger Versuchsaufbau den Zusammenhang zwischen eigener Handlung, lokaler Aktivität und zeitlich passender Rückmeldung nicht korrekt abbildet?**

Erst wenn diese Frage beantwortet ist, wird entschieden, ob eine neue Lernregel, eine verbesserte Exploration oder eine andere neuronale Ausgangsverschaltung erforderlich ist.

---

## Zentraler Forschungsgrundsatz

**Eine Synapse, die ihr Gewicht verändert, hat noch nichts gelernt.**

Erst wenn diese Veränderung dazu beiträgt, dass EVE aufgrund eigener Erfahrungen künftig anders und erfolgreicher handelt, sprechen wir von einem nachgewiesenen Lernprozess.

B.4.1 soll deshalb nicht beweisen, dass unser Drei-Faktor-Ansatz richtig ist.

**B.4.1 soll zuverlässig herausfinden, ob und unter welchen Bedingungen er tatsächlich funktioniert.**
