# Entwicklungsauftrag 022 – Forschungsstufe B.5.0

## Lokale Kreditzuordnung und Stabilisierung positiver Erfahrungen

**Projekt:** EVE – Emergent Virtual Evolution  
**Ausgangsbasis:** `codex/research-stage-b49-local-error-correction`  
**Technologie:** C++17  
**Status:** Kausale Grundlagenforschung mit bedingter lokaler Erweiterung

---

## 1. Ausgangslage

EVE verfügt inzwischen über:

- Ereignisgesteuerte Neuronen mit binären Feuerereignissen.
- Gewichtete erregende und hemmende Synapsen.
- Lokale G3-Ereignisspeicherung.
- Zeitlich begrenzte Eligibility-Spuren.
- Drei-Faktor-Plastizität mit allgemeiner Bewertungsrückmeldung.
- Geschützte angeborene Exploration.
- Lokale inhibitorische Konkurrenz.
- Persistente neuronale Zustände.

Die Forschungsstufen B.4.2 bis B.4.9 konnten zeigen, dass EVE erfahrungsabhängig neuronale Verbindungen verändert und einzelne Reiz-Handlungs-Beziehungen erlernen kann.

Ein zuverlässiges autonomes Lernen mehrerer Beziehungen ist jedoch weiterhin nicht nachgewiesen.

### Erkenntnisse aus B.4.9

B.4.9 untersuchte 122 reale autonome Fehlentscheidungen.

Nach negativer Rückmeldung und erneuter Reizpräsentation:

| Ergebnis | Fälle |
|---|---:|
| Richtige eindeutige Antwort | 74 |
| Keine Antwort | 45 |
| Beide Motorwege aktiv | 3 |
| Wiederholung derselben eindeutigen Fehlantwort | 0 |

Die vorhandene lokale Plastizität verändert falsche Entscheidungen also tatsächlich.

Das Hauptproblem ist nicht die fehlende Wirkung negativer Modulation.

### Positive Alternativerfahrung

In 67 diagnostischen Versuchen wurde eine zusätzliche Explorationshandlung positiv bewertet.

Bei genauerer Betrachtung der Rohdaten:

- In 42 Fällen war die autonome Antwort bereits vor der Exploration richtig.
- In 25 Fällen entstand die positive Handlung erst aus einem zuvor ausbleibenden autonomen Ergebnis.
- Nur 6 dieser 25 Fälle konnten beim nächsten identischen Reiz die richtige Handlung autonom abrufen.

Diese 25 Fälle bilden einen besonders wichtigen Forschungsansatz.

Sie dürfen jedoch nicht als repräsentative Stichprobe aller erfolgreichen Lernerfahrungen interpretiert werden.

### Offener Engpass

EVE kann durch Exploration eine erfolgreiche Handlung ausführen und positive Rückmeldung empfangen.

Noch ungeklärt ist, warum diese Erfahrung häufig nicht zu einer ausreichend stabilen, reizabhängigen neuronalen Verbindung führt.

Insbesondere ist unklar, ob:

1. Die richtigen synaptischen Eingänge überhaupt Eligibility erhalten.
2. Gleichzeitig aktive, aber nicht ursächliche Eingänge ebenfalls verstärkt werden.
3. Die relevante Verbindung trotz positiver Modulation zu schwach bleibt.
4. Die Veränderung einer Beziehung andere bereits erlernte Beziehungen beeinträchtigt.
5. Eine kurzfristig korrekte Antwort durch spätere Erfahrungen wieder verloren geht.

B.5.0 soll diese Ursachen voneinander trennen.

---

## 2. Zentrale Forschungsfrage

**Warum wird eine positiv bewertete, durch Exploration entstandene Handlung nicht zuverlässig als eigenständig abrufbare neuronale Reiz-Handlungs-Beziehung gespeichert?**

Dazu ist der vollständige Übergang zu untersuchen:

**Reiz → keine autonome Antwort → Exploration → erfolgreiche Handlung → positive Modulation → synaptische Veränderung → erneuter Reiz → autonome Antwort**

Der Forschungsgegenstand ist nicht die Auswahl einer richtigen Handlung.

Er ist die lokale Zuordnung und Speicherung einer tatsächlich gemachten Erfahrung.

### Grundprinzip

Die Umwelt darf den Erfolg einer Handlung bewerten.

Das neuronale System darf dadurch jedoch keine Information darüber erhalten, welcher Sensor, Motor oder welche konkrete Synapse der richtigen Lösung entspricht.

Die Lernwirkung muss aus vorhandener neuronaler Aktivität, lokalen synaptischen Zuständen und allgemeiner Modulation entstehen.

---

## 3. Forschungshypothesen

### H1 – Unzureichende lokale Kreditzuordnung

Positive Modulation verändert neben den tatsächlich beteiligten synaptischen Verbindungen weitere lokal berechtigte Kandidaten.

Dadurch können nichtursächliche oder konkurrierende neuronale Wege verstärkt werden.

### H2 – Fehlende Verbindung zwischen Reiz und Exploration

Die Exploration aktiviert zwar einen erfolgreichen Motorweg, aber die vorangegangene sensorische Aktivität wird nicht ausreichend mit der explorativ ausgelösten Entscheidung verbunden.

Eine erfolgreiche motorische Aktivität wäre dann nicht gleichbedeutend mit einer gespeicherten Reiz-Handlungs-Beziehung.

### H3 – Unzureichende synaptische Wirksamkeit

Die richtige Reiz-Handlungs-Verbindung wird verändert, erreicht aber keine ausreichende Wirkung, um später selbstständig ein Feuerereignis auszulösen.

Dies kann mit Feuergrenze, Membranverhalten, konkurrierenden Eingängen oder synaptischer Gewichtung zusammenhängen.

### H4 – Interferenz zwischen Erfahrungen

Die positive Rückmeldung verstärkt Verbindungen, die gleichzeitig an anderen bereits erworbenen Beziehungen beteiligt sind.

Dadurch entstehen konkurrierende oder widersprüchliche neuronale Aktivitätsmuster.

### H5 – Fehlende Stabilität über mehrere Erfahrungen

Eine einzelne positive Erfahrung kann eine korrekte autonome Entscheidung hervorbringen, die bei weiteren Erfahrungen jedoch wieder verlorengeht.

Alle Hypothesen sind getrennt zu prüfen.

Mehrere können gleichzeitig zutreffen.

---

## 4. Phase A – Referenz und methodische Absicherung

Vor jeder funktionalen Änderung sind die bestehenden Ergebnisse zu reproduzieren.

### A1 – Referenzen

Reproduziere:

- Die B.4.9-Referenz K0.
- Die No-Learning-Kontrolle.
- Feste und lernbare inhibitorische Kontrollen.
- Die B.4.8-Explorationsvarianten.
- Normale und inverse Zielzuordnung.
- Drei unabhängige Zufallsstromfamilien.

Die bisherigen 473 Kerntests müssen erhalten bleiben.

### A2 – Eigenständige Evaluation

E1 und E2 werden weiterhin aus getrennten vollständigen Runtime-Kopien ermittelt.

**E1:** Ausschließlich sensorisch ausgelöste autonome Antworten.

**E2:** Geschützte Exploration bei fehlender autonomer Antwort.

Keine weitere Plastizität während der Evaluation.

Die Anfangszustände beider Kopien müssen fingerprint-identisch sein.

### A3 – Trennung von Lernereignissen

Jede positiv bewertete Handlung erhält ausschließlich in der externen Diagnose eine eindeutige Klassifikation:

- Bereits autonom korrekt.
- Zunächst keine autonome Antwort, anschließend durch Exploration korrekt.
- Zunächst autonome Fehlantwort, anschließend durch zusätzliche Aktivität korrekt.
- Doppelte oder mehrdeutige Aktivierung.
- Erfolgloser Explorationsversuch.

**Diese Kategorien dürfen nicht als neuronale Lernzustände verwendet werden.**

Die bereits autonome richtige Antwort ist ausdrücklich von einem erstmalig explorativ erzielten Erfolg zu unterscheiden.

---

## 5. Phase B – Isolierung einer erfolgreichen neuen Erfahrung

Der zentrale Versuch beginnt mit einem neuronalen Zustand, in dem der präsentierte Reiz noch keine autonome richtige Antwort erzeugt.

### B1 – Prospektive Auswahl

Erfasse reale Trainingsereignisse, bei denen:

1. Ein sensorischer Reiz präsentiert wird.
2. Keine autonome Motorantwort entsteht.
3. Die geschützte Exploration aktiviert einen Motorweg.
4. Die äußere Bewertung dieser Handlung positiv ausfällt.

Diese Ereignisse sind ohne gezielte Auswahl günstiger Seeds zu sammeln.

Erfasse sowohl erfolgreiche als auch erfolglose Explorationsereignisse als Kontrollpopulation.

Die bisher bekannten 25 Fälle dienen als historische Referenz, nicht als alleinige Versuchsstichprobe.

### B2 – Zustandskopie vor der Erfahrung

Erzeuge vor dem Ereignis eine vollständige diagnostische Runtime-Kopie.

Sie umfasst:

- Sämtliche Neuronen.
- Sämtliche Synapsen und Gewichte.
- Membran- und Erholungszustände.
- Lokale Ereignisspeicher.
- Eligibility-Spuren.
- Ausstehende Ereignisse.
- Simulierte Zeit.
- RNG-Zustände.

Alle weiteren Varianten starten vom identischen Zustand.

### B3 – Ereignisprotokoll

Erfasse die vollständige Kette:

1. Sensorisches Feuerereignis.
2. Synaptische Impulse.
3. Membranveränderungen.
4. Entstehung lokaler Eligibility.
5. Ausbleibende autonome Antwort.
6. Exploratives Feuerereignis.
7. Aktivierte Entscheidungs- und Motorneuronen.
8. Zusätzliche Eligibility.
9. Positive Modulation.
10. Tatsächliche Gewichtsänderungen.
11. Nächste autonome Reizantwort.

Jede Gewichtsänderung muss einem konkreten lokalen Eligibility-Eintrag und Modulationsereignis zugeordnet werden können.

---

## 6. Phase C – Lokale Kreditzuordnung

Untersuche, welche synaptischen Verbindungen positive Modulation tatsächlich verändert.

### C1 – Kandidateninventur

Für jedes positive Lernereignis erfassen:

- Empfängerneuron.
- Synapsen-ID.
- Sender und Empfänger.
- Erregende oder hemmende Wirkung.
- Gewicht vor der Erfahrung.
- Gewicht nach der Erfahrung.
- Zeitpunkt und Art der lokalen Aktivität.
- Zugehörige Eligibility-Generation.
- Lokale Aktivität vor der Exploration.
- Lokale Aktivität während der Exploration.
- Tatsächlich angewandte Modulation.

Die Diagnose darf nachträglich bestimmen, welche Eingänge an der erfolgreichen motorischen Aktivität beteiligt waren.

Die Lernverarbeitung selbst darf diese externe Klassifikation nicht erhalten.

### C2 – Sensorische Beteiligung

Untersuche, ob die positiven Gewichtsänderungen tatsächlich eine Verbindung vom auslösenden sensorischen Weg zum später erfolgreichen Entscheidungsweg herstellen oder verstärken.

Besonders zu prüfen:

- Sensorische Nullgewicht-Synapsen.
- Bereits schwach aktive sensorische Verbindungen.
- Explorationseingänge.
- Entscheidungsneuronen.
- Inhibitorische Verbindungen.
- Gemeinsam aktive, möglicherweise konkurrierende Eingänge.

### C3 – Nichtursächliche Kandidaten

Ermittle, ob synaptische Verbindungen verstärkt werden, obwohl sie keinen nachweisbaren Beitrag zur erfolgreichen Handlung geleistet haben.

Dabei sind drei unterschiedliche Situationen zu beachten:

**Zeitliche Beteiligung:** Eine Synapse war lediglich innerhalb des Eligibility-Fensters aktiv.

**Funktionale Beteiligung:** Ihre Aktivität beeinflusste den neuronalen Zustand oder die Feuerentscheidung.

**Kausaler Beitrag:** Ein gezielter Eingriff in einer Zustandskopie verändert die erfolgreiche Aktivierung.

Diese drei Eigenschaften dürfen nicht gleichgesetzt werden.

### C4 – Gleichzeitige Verstärkung konkurrierender Wege

Untersuche, ob positive Rückmeldung sowohl den erfolgreichen als auch einen konkurrierenden Reiz-Handlungs-Weg verstärkt.

Prüfe, ob dies später:

- Doppelte Motoraktivierung.
- Falsche autonome Ausgaben.
- Fehlende autonome Ausgaben.
- Verringerte Trennung der Reizbeziehungen

begünstigt.

---

## 7. Phase D – Kausale Zustandskopie-Experimente

Die Kreditzuordnung darf nicht ausschließlich aus zeitlichen Korrelationen abgeleitet werden.

Daher sind gezielte Eingriffe vorzunehmen.

### D0 – Unveränderte Referenz

Identischer Ausgangszustand, vollständige erfolgreiche Erfahrung, unveränderte positive Modulation.

### D1 – Ohne Exploration

Derselbe Ausgangszustand und Reiz, aber ohne den zusätzlichen Explorationsimpuls.

Es erfolgt nur die regulär aus dem tatsächlichen Ergebnis entstehende Rückmeldung.

### D2 – Exploration ohne wirksame Plastizität

Dieselbe explorative Aktivität findet statt, die anschließende Gewichtsanpassung bleibt deaktiviert.

### D3 – Neutrale statt positiver Modulation

Identische neuronale Aktivität, jedoch diagnostisch neutrale Modulation.

Diese Variante untersucht den Beitrag positiver Gewichtsänderungen.

### D4 – Gezielte Rücknahme einzelner Gewichtsänderungen

Nach einer positiven Erfahrung werden in getrennten diagnostischen Kopien die Veränderungen einzelner Synapsen zurückgenommen.

Unterscheide insbesondere:

- Sensorische Eingänge.
- Explorative Eingänge.
- Inhibitorische Eingänge.
- Eingänge anderer Entscheidungswege.

### D5 – Zeitlich getrennte Modulation

Variiere den zeitlichen Abstand zwischen erfolgreicher Handlung und positiver Modulation.

Untersuche, welche Eligibility-Kandidaten dadurch erhalten bleiben oder ablaufen.

### D6 – Diagnose der Kandidatenmenge

Prüfe, ob eine reduzierte oder veränderte Auswahl lokal berechtigter Kandidaten das Verhalten beeinflusst.

Solche Manipulationen erfolgen zunächst ausschließlich in diagnostischen Kopien.

Es darf nicht aufgrund der extern bekannten richtigen Handlung ein bestimmter Kandidat ausgewählt werden.

### D7 – Fortsetzung der Erfahrung

Nach den Eingriffen wird derselbe Reiz erneut präsentiert.

Erfasse E1 ohne Exploration und ohne weitere Plastizität.

Anschließend sind identische zukünftige Erfahrungsfolgen auf getrennten Zustandskopien auszuwerten.

Ziel ist der Nachweis, welche konkreten Veränderungen zur Entstehung beziehungsweise zum Verlust der autonomen Antwort beitragen.

---

## 8. Phase E – Warum wird eine erfolgreiche Verbindung nicht autonom?

Untersuche die neuronale Wirksamkeit der nach positiver Bewertung entstandenen Verbindung.

### E1 – Sensorische Gewichtung

Erfasse:

- Gewicht des erfolgreichen sensorischen Pfades.
- Gewicht des konkurrierenden Pfades.
- Abstand der jeweiligen Aktivität zur Feuergrenze.
- Angewandte inhibitorische Beiträge.
- Aktivität der Entscheidungsneuronen.
- Aktivität der motorischen Ausgänge.

### E2 – Fehlende autonome Antwort

Unterscheide Fälle, in denen:

- Kein sensorischer Eingang funktional wirksam ist.
- Ein Eingang aktiv wird, aber unterhalb der Feuergrenze bleibt.
- Beide Entscheidungswege aktiviert werden.
- Inhibitorische Aktivität eine Entscheidung unterdrückt.
- Eine Entscheidung erfolgt, aber der falsche Motor aktiviert wird.

### E3 – Synaptische Wirksamkeit und Wiederholung

Untersuche, wie sich eine weitere reale positive Erfahrung auf dieselbe Verbindung auswirkt.

Prüfe:

- Entsteht eine eigenständige Antwort nach einer weiteren positiven Erfahrung?
- Bleibt sie anschließend bestehen?
- Führt wiederholte positive Bewertung zur Sättigung?
- Werden gleichzeitig konkurrierende Wege verstärkt?
- Beeinträchtigt dies die zweite Reizbeziehung?

Eine gezielt im Experiment herbeigeführte positive Bewertung ohne tatsächlich erfolgreiche Handlung darf nicht als natürlicher Lernerfolg gewertet werden.

---

## 9. Phase F – Erhalt der zweiten Erfahrung

Eine erfolgreich gelernte neue Beziehung ist nicht ausreichend, wenn dadurch eine andere bereits erworbene Beziehung verloren geht.

### F1 – Bestehende Beziehung vor dem Ereignis

Prüfe vor der neuen positiven Erfahrung den autonomen Erfolg des anderen Reizes.

### F2 – Veränderung durch positive Modulation

Erfasse, ob die neue positive Erfahrung Synapsen verändert, die auch von der anderen Beziehung benötigt werden.

### F3 – Unmittelbare Interferenz

Präsentiere anschließend beide Reize getrennt und ohne weitere Plastizität.

Untersuche:

- Neue Beziehung autonom abrufbar?
- Vorherige Beziehung weiterhin abrufbar?
- Doppelaktivierungen?
- Fehlende Ausgaben?
- Veränderung gemeinsamer oder konkurrierender Gewichte?

### F4 – Längerfristige Interferenz

Setze das Training mit gemischten Erfahrungen fort.

Prüfe, ob zunächst entstandene getrennte Beziehungen weiterhin bestehen bleiben.

### F5 – Kausale Eingriffe

Nimm in diagnostischen Zustandskopien einzelne positive Gewichtsänderungen zurück.

Ermittle, welche Änderungen für den neuen Erfolg notwendig sind und welche die andere Beziehung beeinträchtigen.

---

## 10. Phase G – Bedingte Forschungsvariante

**Ein neuer neuronaler Lernmechanismus darf erst eingeführt werden, wenn die Phasen B bis F einen konkreten lokalen Fehler kausal belegen.**

Eine Verbesserung der Zielquote allein ist keine ausreichende Begründung.

### Mögliche Untersuchung

Falls nachgewiesen wird, dass die bisherige Eligibility-Auswahl regelmäßig nichtursächliche Synapsen verstärkt und dadurch erfolgreiches Lernen verhindert, darf eine minimale alternative lokale Kandidatenzuordnung untersucht werden.

Die konkrete Regel wird erst anhand der Diagnose festgelegt.

Dabei müssen gelten:

- Keine externen Reizkennungen.
- Keine motorischen Ziel-IDs als Lerninformation.
- Keine technisch vorgegebene richtige Handlung.
- Keine Auswahl anhand der bekannten richtigen Synapse.
- Keine globale neuronale Zustandsabfrage.
- Keine pauschale Löschung bestehender Erfahrungen.
- Keine neue Lernregel ohne isolierten Wirkungsnachweis.

Eine alternative lokale Zuordnung darf ausschließlich Informationen verwenden, die im natürlichen neuronalen Ereignisablauf tatsächlich verfügbar sind.

### Minimalitätsprinzip

Zunächst ist genau eine begründete Veränderung als Forschungsvariante zulässig.

Die übrigen Lernbedingungen müssen identisch bleiben.

### Implementierungsort

Falls ein neuer lokaler Mechanismus notwendig wird, ist seine Einordnung in neuronalen Kern oder Stammhirn zu begründen.

Ein während des Lernens relevanter neuer Zustand darf nicht lediglich als versteckte Variable des Forschungsrunners existieren.

Neue Zustände benötigen vollständige Persistenz, kontrollierbare Abschaltung und deterministische Fortsetzung.

### Keine automatische Freigabe

B.5.0 darf auch ohne neue Implementierung abgeschlossen werden.

Ein negativer oder uneindeutiger Kausalbefund ist ausdrücklich zulässig.

---

## 11. Kontrollgruppen

Mindestens folgende Gruppen sind zu untersuchen:

| Gruppe | Beschreibung |
|---|---|
| K0 | Unveränderte B.4.9-Referenz |
| K1 | No-Learning |
| K2 | Feste inhibitorische Wirkung |
| K3 | Lernbare inhibitorische Wirkung |
| K4 | Exploration ohne positive Plastizität |
| K5 | Positive Erfahrung mit unveränderter Eligibility-Auswahl |
| K6 | Diagnostisch isolierte sensorische Gewichtsänderungen |
| K7 | Diagnostisch isolierte konkurrierende Gewichtsänderungen |
| K8 | Zeitlich veränderte positive Modulation |
| K9 | Bedingte minimale lokale Forschungsvariante |
| K10 | Identischer Aufbau mit deaktivierter neuer Funktion |

K9 und K10 sind nur erforderlich, falls eine neue Funktion kausal begründet wird.

Alle diagnostisch selektiven Eingriffe müssen ausdrücklich als solche gekennzeichnet werden.

Sie sind keine zulässige neuronale Lernstrategie.

---

## 12. Lernaufgaben

Die zentralen Varianten durchlaufen:

1. Einzelreiz X→A.
2. Einzelreiz Y→B.
3. X→A, danach Y→B.
4. Y→B, danach X→A.
5. Streng alternierende Erfahrungen.
6. Zufällig gemischte Erfahrungen.
7. Inverse Zuordnung X→B und Y→A.
8. Umweltumkehr ohne neuronalen Reset.
9. Retention ohne weitere Plastizität.

### Zusätzliche Aufgaben für B.5.0

**P1 – Erstmalig erfolgreiche Exploration**

Vorher keine autonome Antwort, anschließend positive explorative Erfahrung.

**P2 – Bereits autonome richtige Handlung**

Positive Bewertung ohne Notwendigkeit zusätzlicher Exploration.

**P3 – Erfolgreiche Alternative nach negativer Erfahrung**

Eine zuvor falsche oder ausbleibende Entscheidung wird durch eine neue positiv bewertete Erfahrung ergänzt.

**P4 – Positive Erfahrung bei bereits gelernter zweiter Beziehung**

Untersuchung der synaptischen Interferenz.

**P5 – Wiederholte positive Erfahrungen**

Untersuchung der Entstehung, Sättigung und Stabilität einer autonomen Verbindung.

---

## 13. Primäre Erfolgsmessung

### E1 – Autonomer neuronaler Erfolg

Die korrekte Handlung muss ausschließlich durch den präsentierten Reiz und das bereits trainierte neuronale Netzwerk entstehen.

Während der E1-Evaluation gelten:

- Keine zusätzliche Exploration.
- Keine weitere Plastizität.
- Keine technische Handlungsentscheidung.
- Kein Reset neuronaler Zustände.

Doppelte und fehlende Ausgaben zählen als Misserfolg.

### E2 – Explorationsgestützter Erfolg

Die bekannte geschützte Exploration wird separat ausgewertet.

E2 ist eine sekundäre Messgröße.

### Zusätzliche B.5.0-Messgrößen

Für jedes echte positive neue Lernereignis:

- Anteil mit anschließend korrektem E1-Abruf.
- Anteil mit weiterhin falschem Abruf.
- Anteil ohne autonome Ausgabe.
- Anteil mit Doppelaktivierung.
- Anteil mit gleichzeitig erhaltener zweiter Beziehung.
- Anzahl tatsächlich veränderter synaptischer Kandidaten.
- Anteil kausal relevanter Gewichtsänderungen gemäß diagnostischer Ablation.
- Stabilität nach weiteren Erfahrungen.

Die Auswertung muss nach vorherigem autonomem Ergebnis getrennt erfolgen.

Bereits autonom richtige Antworten dürfen nicht in die Quote erstmalig erworbener autonomer Fähigkeiten eingerechnet werden.

---

## 14. Allgemeine Erfolgskriterien

Das übergeordnete Forschungsziel bleibt zuverlässiges autonomes Mehrfachlernen.

Erforderlich sind:

- Mindestens 75 % E1-Erfolg für X.
- Mindestens 75 % E1-Erfolg für Y.
- Mindestens 48/64 Seeds bestehen beide Schwellen gleichzeitig.
- Mindestens 15 Prozentpunkte gepaarter Lernvorteil gegenüber No-Learning.
- Vollständig positives 95-%-Konfidenzintervall.
- Bestätigung mit 64 unbenutzten Seeds.
- Reproduktion über mindestens drei unabhängige Zufallsstromfamilien.
- Erfolg bei normaler und inverser Zielzuordnung.
- Retention ohne weitere Plastizität.

### Spezifisches Forschungsziel B.5.0

Unabhängig von der allgemeinen Erfolgsquote soll kausal geklärt werden:

**Welche lokale synaptische Veränderung ist notwendig und hinreichend, damit eine zuvor ausschließlich durch Exploration erfolgreiche Handlung später autonom ausgelöst werden kann?**

Falls notwendige und hinreichende Bedingungen nicht vollständig identifiziert werden können, sind die nachgewiesenen Teilbedingungen und verbleibenden Unsicherheiten zu dokumentieren.

Es darf keine allgemeine Lernfreigabe aus einem erfolgreichen isolierten Diagnosefall abgeleitet werden.

---

## 15. Statistische Anforderungen

### Hauptversuche

Mindestens:

- 64 gepaarte Entdeckungs-Seeds je Stromfamilie und Zielzuordnung.
- 64 zusätzliche unabhängige Bestätigungs-Seeds.
- Drei unabhängig erzeugte Erfahrungsstromfamilien.
- Normale und inverse Zielzuordnung.
- Reproduzierbare sensorische, explorative und neuronale Zufallsquellen.

### Positivereignis-Stichprobe

Die diagnostische Stichprobe erstmalig positiver Explorationshandlungen ist prospektiv und ohne Erfolgsauswahl zu erheben.

Erfasse alle zulässigen Ereignisse der vorab definierten Untersuchungsgruppe.

Sollte die Stichprobe zu klein sein, dürfen zusätzliche vorab festgelegte Seeds oder Läufe verwendet werden.

Die Anzahl und Auswahlregeln müssen dokumentiert sein.

Mehrere Ereignisse desselben Seeds oder Netzwerks dürfen nicht als unabhängige Stichproben behandelt werden.

### Auswertung

Verwende:

- Seedbezogene Erfolgsquoten.
- Ereignisbezogene Diagnosen.
- Gepaarte Zustandskopie-Vergleiche.
- Clusterbewusste Bootstrap-Konfidenzintervalle bei mehreren Ereignissen je Seed.
- Verwechslungsmatrizen.
- Synaptische Gewichtsentwicklung.
- Zeitliche Eligibility-Protokolle.
- Kausale Ablationsvergleiche.
- Unabhängige Bestätigung.

Die Bestätigungsdaten dürfen nicht für die Entwicklung oder Auswahl einer neuen Lernregel genutzt werden.

---

## 16. Architekturgrenzen

Unverändert gelten:

- C++17.
- Vollständig ereignisgesteuerte Verarbeitung.
- Kein globaler neuronaler Tick.
- Binäre neuronale Feuerereignisse.
- Gewichtete erregende und hemmende Eingänge.
- Lokale G3- und Eligibility-Verarbeitung.
- Drei-Faktor-Plastizität.
- Geschützte angeborene Exploration.
- Keine globale Motorselektion.
- Keine versteckte Winner-take-all-Regel.
- Keine äußeren semantischen Reizinformationen im neuronalen Lernzustand.
- Keine technisch vorgegebene richtige Handlung.
- Keine direkten Ziel-Synapsen-Anweisungen aus der Umweltbewertung.
- Keine permanenten Zeitstempel pro Synapse.
- Keine automatische Löschung erworbener Erfahrungen.
- Keine unkontrollierte Netzvergrößerung.
- Vollständige Persistenz.
- Reproduzierbare und deterministische Verarbeitung.

Synapsenwachstum bleibt in den primären Untersuchungen deaktiviert.

Diagnostische Eingriffe dürfen gezielt neuronale Zustände verändern, müssen jedoch vollständig von zulässigen Lernmechanismen getrennt bleiben.

---

## 17. Regression und technische Validierung

Alle bisherigen 473 Kerntests müssen weiterhin bestehen.

Neue Tests sind erforderlich für:

- Identische Runtime-Zustandskopien.
- E1-only- und E2-Evaluation.
- Erste positive Explorationshandlung.
- Eligibility-Kandidaten vor und nach Exploration.
- Positive Modulation auf Nullgewicht-Synapsen.
- Synaptische Gewichtungsgrenzen.
- Gleichzeitige aktive konkurrierende Kandidaten.
- Kausale Rücknahme einzelner Gewichtsänderungen.
- Negative und positive Modulation innerhalb einer Ereigniskette.
- Erhalt einer zweiten Reizbeziehung.
- Inhibitorische Stabilität.
- Ereignisreihenfolge.
- Persistenz neu eingeführter Zustände.
- Abschaltkontrolle neuer Funktionen, sofern vorhanden.
- B.4.2- bis B.4.9-Verhaltensregressionen.

Technische Fehler, ungültige neuronale Zustände und abgebrochene Läufe dürfen nicht stillschweigend aus den Erfolgsquoten entfernt werden.

---

## 18. Lieferumfang

Nova-EVE erstellt:

1. Technische Analyse der bisherigen positiven Kreditzuordnung.
2. Exakte Reproduktion der B.4.9-Referenzen.
3. Prospektive Stichprobe erstmalig positiver Explorationshandlungen.
4. Vollständige lokale Ereignis- und Eligibility-Protokolle.
5. Analyse aller tatsächlich modifizierten synaptischen Kandidaten.
6. Kausale Zustandskopie-Ablationen.
7. Getrennte Auswertung sensorischer, explorativer und inhibitorischer Beiträge.
8. Untersuchung der autonomen Wirksamkeit neuer Gewichte.
9. Untersuchung der Interferenz mit vorhandenen Reizbeziehungen.
10. Vergleich einzelner und wiederholter positiver Erfahrungen.
11. Gegebenenfalls eine minimal begründete lokale Forschungsvariante.
12. E1-/E2-Auswertung normaler und inverser Lernaufgaben.
13. Unabhängige Bestätigungsversuche.
14. Vollständige Rohdaten und statistische Auswertung.
15. Regression, Stabilitäts- und Persistenzprüfungen.
16. Forschungsbericht B.5.0.
17. Entscheidungsvorlage für B.5.1.

Bisherige Forschungsdaten bleiben unverändert erhalten.

---

## 19. Abschlussentscheidung

Am Ende sind folgende Fragen zu beantworten:

**A.** Welche synaptischen Eligibility-Spuren erhalten nach einer erstmalig erfolgreichen Explorationshandlung positive Modulation?

**B.** Welche dieser Verbindungen haben tatsächlich kausalen Einfluss auf den späteren autonomen Abruf?

**C.** Werden gleichzeitig nichtursächliche oder konkurrierende Verbindungen verstärkt?

**D.** Wird die Verbindung zwischen sensorischem Reiz und erfolgreicher explorativer Handlung ausreichend gespeichert?

**E.** Liegt ein Teil des Problems an der neuronalen Feuergrenze beziehungsweise unzureichender synaptischer Wirksamkeit?

**F.** Warum beeinträchtigt eine positive Erfahrung gegebenenfalls bereits erworbene andere Beziehungen?

**G.** Reichen die bestehenden lokalen Mechanismen grundsätzlich aus, sofern sie geeignete Bedingungen vorfinden?

**H.** Ist eine neue lokale Kreditzuordnung tatsächlich notwendig und kausal begründbar?

**I.** Kann eine nachweislich erfolgreiche neue Bindung über weitere Erfahrungen hinweg erhalten bleiben?

**J.** Welche Bedeutung haben die Befunde für das spätere autonome Mehrfachlernen und Umlernen?

Eine neue allgemeine Lernfunktion darf nur aufgrund eines nachvollziehbaren, isolierten Kausalbefunds freigegeben werden.

Ein negativer Forschungsbefund ist ausdrücklich zulässig.

---

## 20. Forschungsleitlinie

**Positive Rückmeldung ist noch kein gelerntes Wissen.**

Sie verändert neuronale Zustände. Ob daraus eine zuverlässig abrufbare Fähigkeit entsteht, hängt davon ab, welche Verbindungen verändert werden und wie diese später zusammenwirken.

EVE darf nicht technisch mitgeteilt bekommen, welche Synapse den Erfolg speichern soll.

Das neuronale System muss anhand seiner eigenen lokalen Aktivität und der erfahrenen Umweltwirkung geeignete Verbindungen verstärken können.

Dabei dürfen bereits erworbene Fähigkeiten nicht unnötig zerstört werden.

**Das Ziel von B.5.0 ist nicht, eine korrekte Handlung zu erzeugen. Das Ziel ist zu verstehen, wie eine tatsächlich erfolgreiche Handlung zu einer dauerhaft verfügbaren neuronalen Fähigkeit wird.**
