# Entwicklungsauftrag 036 – Forschungsstufe B.5.12

## Teillernen, latentes Lernen, Verfestigung und die Entwicklung neuronaler Fähigkeiten

**Projekt:** EVE – Emergent Virtual Evolution
**Technologie:** C++17
**Forschungsbranch:** `codex/research-stage-b512-learning-acquisition-and-consolidation`
**Ausgangsbasis:** Vollständig korrigierter und integrierter Stand B.5.11
**Forschungstyp:** Grundlagenforschung und Validierung der Lernbewertung
**Neue neuronale Produktionsregel:** Nicht freigegeben

---

## 1. Verbindliche Ausgangslage

B.5.11 hat gezeigt, dass unsere bisherige Bewertung neuronalen Lernens unzureichend sein kann.

Insbesondere dürfen folgende Eigenschaften nicht gleichgesetzt werden:

1. Neuronale Aktivität.
2. Neuronale Veränderung.
3. Entstehende funktionale Vorstrukturierung.
4. Erster erfolgreicher Abruf.
5. Zuverlässig abrufbare Fähigkeit.
6. Langfristig verfügbare Fähigkeit.

Bisher wurde ein Lernerfolg häufig anhand einer unmittelbar korrekten Motorantwort festgestellt.

Diese Betrachtung greift möglicherweise zu kurz.

**Eine Information kann bereits teilweise neuronal verarbeitet und erlernt worden sein, bevor daraus eine unmittelbar nutzbare Fähigkeit entsteht.**

Darüber hinaus können weitere Erfahrungen notwendig sein, damit die entsprechende Fähigkeit zuverlässig abrufbar wird.

B.5.12 untersucht deshalb den Lernprozess selbst und nicht nur dessen momentan sichtbares Ergebnis.

### Verbindlicher Forschungsgrundsatz

**Neuronale Veränderung ist nicht automatisch Lernen. Fehlende Verhaltensleistung ist nicht automatisch fehlendes Lernen. Und die erste richtige Antwort ist nicht automatisch der Abschluss eines Lernprozesses.**

Diese Unterscheidung muss in sämtlichen Versuchen und Auswertungen berücksichtigt werden.

---

## 2. Voraussetzung: Korrigierte B.5.11

Die laufenden B.5.11-Korrekturen müssen vor Beginn der eigentlichen B.5.12-Experimente vollständig abgeschlossen sein.

Verbindlich sind:

- Korrekte Trennung produktiver, nachteiliger und unentschiedener Reorganisationsereignisse.
- Unterscheidung positiver und negativer Gewichtsänderungen.
- Kausale Prüfung unmittelbarer Abrufbeeinträchtigungen.
- Explorationsbewertung anhand tatsächlich ausgeführter Motorhandlungen.
- Aktualisierte Forschungsdaten und Dokumentation.
- Vollständig bestandene Regressionstests.
- Persistenz und deterministische Fortsetzung.

B.5.12 muss vom anschließend integrierten `main` ausgehen.

Die bisher für B.5.12 vorgesehene Untersuchung zeitlicher Kreditbindung wird zunächst auf **B.5.13** verschoben.

---

## 3. Zentrale Forschungsfrage

**Kann EVE durch Erfahrungen bereits eine funktional relevante neuronale Vorstrukturierung erwerben, obwohl sie die entsprechende Fähigkeit noch nicht oder nur unzuverlässig ausführen kann?**

Ergänzend:

**Wie entwickelt sich eine zunächst teilweise verfügbare Fähigkeit durch weitere Erfahrungen zu einer zuverlässiger und unter mehr Bedingungen abrufbaren Fähigkeit?**

Dabei dürfen keine bestimmten synaptischen Veränderungen als notwendige Lernform vorausgesetzt werden.

Die Forschung muss ergebnisoffen untersuchen, ob relevante Veränderungen durch:

- Verstärkung vorhandener Synapsen.
- Abschwächung vorhandener Synapsen.
- Kombination und Wiederverwendung vorhandener Verbindungen.
- Veränderte erregende und hemmende Zusammenarbeit.
- Zeitliche neuronale Zustände.
- Weitere natürliche Erfahrungen.

entstehen.

---

## 4. Verbindliche Begriffsdefinitionen

### 4.1 Neuronale Aktivität

Ein tatsächlich beobachtbarer neuronaler Verarbeitungsvorgang.

Aktivität allein beweist kein Lernen.

### 4.2 Neuronale Veränderung

Eine durch Erfahrungen entstandene Änderung eines neuronalen Zustands oder einer Verbindung.

Eine Veränderung allein beweist noch keinen funktionalen Nutzen.

### 4.3 Teillernen

Ein Zustand, in dem frühere Erfahrungen bereits nachweislich aufgabenspezifische funktionale Auswirkungen besitzen, obwohl die vollständige geforderte Fähigkeit noch nicht zuverlässig ausgeführt werden kann.

**Teillernen darf nicht allein anhand veränderter Gewichte diagnostiziert werden.**

### 4.4 Latentes Lernen

Eine durch frühere Erfahrungen entstandene funktionale Vorstrukturierung, die unter der aktuellen Prüfung noch nicht als erfolgreiche Handlung erscheint, aber durch geeignete spätere Gegenproben nachweisbar wird.

Eine solche Diagnose benötigt einen Vergleich mit passenden Kontrollgruppen.

### 4.5 Erster erfolgreicher Abruf

Das erstmalige Auftreten einer tatsächlich korrekten autonomen Antwort.

Dieser Zeitpunkt ist ein beobachtbares Ereignis, aber kein ausreichender Beweis für stabile Beherrschung.

### 4.6 Verfestigung

Eine nachweisbare Verbesserung der Zuverlässigkeit, Kontextunabhängigkeit oder Beständigkeit einer Fähigkeit durch weitere neuronale Verarbeitung oder Erfahrungen.

Verfestigung ist **nicht** mit einer bloßen Erhöhung synaptischer Gewichte gleichzusetzen.

### 4.7 Retention

Die Verfügbarkeit einer zuvor nachgewiesenen Fähigkeit nach einer bestimmten Zeit oder nach weiteren Erfahrungen.

### 4.8 Wiedererwerb

Die erneute Entwicklung einer aktuell nicht zuverlässig abrufbaren Fähigkeit.

Ein schnellerer Wiedererwerb kann auf erhaltene neuronale Vorstrukturierung hinweisen.

---

## 5. Forschungshypothesen

### H1 – Lernen vor der ersten korrekten Antwort

EVE kann durch Erfahrungen funktionale neuronale Veränderungen erwerben, obwohl noch keine richtige autonome Handlung entsteht.

### H2 – Beschleunigtes weiteres Lernen

Bereits trainierte, aber aktuell erfolglose Netze können bei identischen weiteren Lernerfahrungen schneller Fortschritte erzielen als unerfahrene Netze.

### H3 – Aufgabenspezifische Vorstrukturierung

Vorherige Erfahrungen mit A erleichtern den späteren Erwerb von A stärker als gleich viele Erfahrungen mit einer unabhängigen Kontrollaufgabe.

### H4 – Erste Antwort ist nicht Beherrschung

Der erste autonome Erfolg sagt die spätere Abrufzuverlässigkeit nicht hinreichend voraus.

### H5 – Verfestigung durch weitere Erfahrungen

Weitere Erfahrungen können die Zuverlässigkeit einer Fähigkeit verbessern, ohne dass dafür eine bestimmte Synapse unverändert erhalten bleiben muss.

### H6 – Verfestigung durch Reorganisation

Weitere Gewichtsänderungen können die zuverlässige Abrufbarkeit verbessern, obwohl bereits beteiligte Synapsen dabei abgeschwächt oder für andere Aufgaben wiederverwendet werden.

### H7 – Pausen und zeitliche Zustände

Die Abrufbarkeit kann sich zwischen zwei Prüfungen durch vorhandene transiente oder ausstehende lokale Ereignisse verändern.

Eine Verbesserung allein durch Zeitablauf ohne entsprechende neuronale Zustandsverarbeitung darf nicht vorausgesetzt werden.

### H8 – Vorübergehend fehlender Abruf

Eine zuvor erfolgreich ausgeführte Fähigkeit kann zeitweilig nicht abrufbar sein, ohne dass ihre gesamte funktionale Vorstrukturierung verloren gegangen ist.

### H9 – Unterschiedliche Lernstadien

Neuronale Aktivität, erste funktionale Vorbereitung, erster Abruf und zuverlässige Beherrschung können zu unterschiedlichen Zeitpunkten auftreten.

Alle Hypothesen sind offen zu prüfen.

---

## 6. Phase A – Forschungsumgebung und Aufgaben

Verwende die korrigierte B.5.11-Forschungsumgebung als Ausgangspunkt.

Erhalten bleiben:

- Die bisherigen X/Y-Referenzen.
- Die zeitlichen Aufgaben A und B.
- Die gemeinsamen sensorischen Bestandteile.
- Die unabhängigen Kontrollaufgaben.
- Die Transferaufgabe C.
- Die vorhandenen neuronalen Lernmechanismen.
- Die geschützte Exploration.

Eine zusätzliche einfache, nachweislich erlernbare Kontrollaufgabe ist zulässig, falls die zeitlichen B.5.11-Aufgaben für bestimmte Untersuchungen zu schwierig sind.

### Verbindliche Bedingung

Der Forschungsauftrag darf nicht daran scheitern, dass eine untersuchte Aufgabe mit der vorhandenen Architektur überhaupt nicht erlernt werden kann.

Deshalb müssen einfache Referenzaufgaben und schwierige Mehrfachlernaufgaben getrennt untersucht werden.

Die Aufgaben dürfen jedoch nicht nachträglich anhand günstiger Ergebnisse umgestaltet werden.

---

## 7. Phase B – Vollständige Lernkurven statt einzelner Endzustände

Untersuche den gesamten Lernverlauf vom unerfahrenen Zustand bis zum Ende des Trainings.

### Kontrollpunkte

Mindestens nach:

0, 1, 2, 4, 8, 16, 32, 64, 128, 256 und 512 Erfahrungen.

Bei Bedarf zusätzliche ereignisnahe Beobachtungen zwischen diesen Punkten.

### Zu erfassen

- Tatsächliche autonome Motorantworten.
- Fehlende Antworten.
- Falsche Antworten.
- Doppelantworten.
- Vorzeitige Antworten.
- Explorative Handlungen.
- Tatsächlich erhaltene Modulation.
- Positive und negative Gewichtsänderungen.
- Lokale neuronale Aktivität.
- Membran- und Eligibility-Zustände.
- Abrufbarkeit unter verschiedenen gültigen Kontexten.

### Wichtig

Die Prüfung erfolgt auf vollständigen Zustandskopien.

**Die diagnostischen Prüfungen dürfen den ursprünglichen Lernverlauf nicht verändern.**

Es ist zwischen der Verhaltensentwicklung innerhalb des natürlichen Trainings und den Ergebnissen separater diagnostischer Abrufprüfungen zu unterscheiden.

---

## 8. Phase C – Lernen ohne erkennbare richtige Antwort

Diese Phase untersucht, ob bereits erworbene neuronale Vorstrukturierung nachgewiesen werden kann, bevor EVE erstmals korrekt autonom handelt.

### C1 – Versuchsgruppe

EVE erhält eine festgelegte Anzahl natürlicher Erfahrungen mit Aufgabe A.

### C2 – Unerfahrene Kontrolle

Eine identisch initialisierte Kopie erhält noch keine A-Erfahrungen.

### C3 – Unabhängige Erfahrungskontrolle

Eine weitere Kopie erhält dieselbe Anzahl vergleichbarer Erfahrungen mit einer anderen Aufgabe.

### C4 – Zeitkontrolle

Eine weitere Kopie durchläuft denselben zeitlichen Zeitraum ohne A-Lernerfahrungen.

### C5 – Natürliche Modulation

Rückmeldungen entstehen ausschließlich aus tatsächlich ausgeführten Handlungen.

### C6 – Weiterlernen

Anschließend erhalten sämtliche Gruppen dieselbe vorab festgelegte Folge weiterer A-Erfahrungen.

Die Kopien müssen ihre tatsächlichen eigenen Handlungen und Rückmeldungen erzeugen.

### C7 – Messung

Untersuche:

- Bis zur ersten autonomen korrekten Antwort benötigte zusätzliche Erfahrungen.
- Entwicklung der korrekten Antwortrate.
- Entwicklung der bedingten Abrufbarkeit.
- Notwendige zusätzliche Gewichtsänderungen.
- Verhalten nach weiteren Erfahrungen.

### C8 – Kausalinterpretation

Wenn die zuvor mit A trainierten Netze anschließend besser oder schneller lernen, obwohl sie vor der Fortsetzung keine nachweisbare erfolgreiche Antwort erzeugten, ist dies ein Hinweis auf funktionale Vorstrukturierung.

**Eine bloße Gewichtsänderung reicht für diesen Nachweis nicht aus.**

---

## 9. Phase D – Tatsächlich latentes Lernen untersuchen

Für diese Phase werden insbesondere Netze betrachtet, die nach einem vorab festgelegten Trainingsabschnitt noch keine erfolgreiche autonome A-Antwort erzeugen.

### D1 – Auswahl

Die Kriterien für fehlenden Abruf müssen vor dem Versuch festgelegt werden.

### D2 – Gegenprobe

Vergleiche solche Netze mit entsprechend kontrollierten unerfahrenen und anders trainierten Netzen.

### D3 – Gleiche weitere Erfahrungen

Alle Gruppen erhalten dieselben weiteren A-Eingangsfolgen.

### D4 – Messung

Prüfe, ob die bereits vorhandene neuronale Vorstrukturierung den weiteren Lernverlauf kausal beeinflusst.

### D5 – Kontrollierte Zustände

Unterscheide:

- Synaptische Gewichte.
- Transiente Membranzustände.
- Eligibility.
- Ausstehende Ereignisse.
- Zufallszustände.

### D6 – Keine voreilige Schlussfolgerung

Ein verändertes Netz, das noch nicht richtig antwortet, ist nicht automatisch ein latent lernendes Netz.

Dafür muss eine funktionale Wirkung seiner bisherigen Erfahrungen nachgewiesen werden.

### D7 – Selektionsverzerrung vermeiden

Da nur Netze ohne bisherigen erfolgreichen Abruf betrachtet werden, sind diese Ergebnisse zusätzlich zur vollständigen, vorab definierten Population auszuwerten.

Eine Auswahl nach später bekanntem Erfolg ist unzulässig.

---

## 10. Phase E – Vom ersten Abruf zur zuverlässigen Fähigkeit

Der erste autonome Erfolg wird ausdrücklich **nicht** als Abschluss des Lernens gewertet.

### E1 – Erster Abruf

Erfasse den Zeitpunkt der ersten richtigen autonomen Antwort.

### E2 – Weiteres Training

Führe die natürliche Lernerfahrung unverändert fort.

### E3 – Zuverlässigkeit

Prüfe, ob die Fähigkeit in zunehmend vielen gültigen Situationen korrekt ausgeführt wird.

### E4 – Kontext

Untersuche:

- Unterschiedliche vorherige Erfahrungen.
- Unterschiedliche zeitliche Abstände.
- Unterschiedliche zulässige Eingangsfolgen.
- Verschiedene transiente Ausgangszustände.

### E5 – Stabile Beherrschung

Definiere vorab konkrete aufgabenbezogene Kriterien für wiederholt zuverlässigen Abruf.

Ein einzelner Erfolg erfüllt diese Kriterien nicht.

### E6 – Keine universelle Schwelle

Die für X/Y verwendeten historischen Robustheitsschwellen dürfen nicht unbesehen auf neue Aufgaben übertragen werden.

### E7 – Unterscheidung

Berichte getrennt:

- Erster autonomer Erfolg.
- Wiederholter Erfolg.
- Zuverlässiger bedingter Abruf.
- Retention nach weiteren Erfahrungen.

---

## 11. Phase F – Verfestigung durch weitere Erfahrungen

Vergleiche vollständige Kopien desselben bereits trainierten Zustands.

### F0 – Ausgangszustand

Alle Kopien besitzen denselben vollständigen neuronalen Zustand.

### F1 – Weitere passende Erfahrungen

Die erste Kopie erhält zusätzliche natürliche Erfahrungen mit A.

### F2 – Keine weiteren Lernerfahrungen

Die zweite Kopie durchläuft nur den entsprechenden Zeitraum.

### F3 – Andere Erfahrungen

Die dritte Kopie erhält eine vergleichbare Anzahl unabhängiger Erfahrungen.

### F4 – Gemischte Erfahrungen

Die vierte Kopie erhält weitere Erfahrungen mit A und einer zweiten Aufgabe.

### F5 – Messung

Prüfe:

- Zuverlässigkeit des A-Abrufs.
- Kontextabhängigkeit.
- Zeitliche Beständigkeit.
- Wiedererwerb.
- Veränderung vorhandener Gewichte.
- Wiederverwendung vorhandener neuronaler Strukturen.

### F6 – Begriffliche Grenze

Eine Verbesserung durch zusätzliche A-Erfahrungen ist zunächst **erfahrungsabhängige Verfestigung**.

Eine Verbesserung während einer Pause darf nur dann als mögliche erfahrungsfreie Konsolidierung interpretiert werden, wenn tatsächlich lokale neuronale Prozesse stattfinden, die diese Veränderung erklären können.

Das bloße Fortschreiten der Simulationszeit ist kein Lernnachweis.

---

## 12. Phase G – Verfestigung ist nicht Gewichtsschutz

Untersuche, ob eine zunehmend zuverlässig abrufbare Fähigkeit mit weiteren Veränderungen ihrer bereits verwendeten Synapsen einhergeht.

### G1 – Gewichtsverlauf

Erfasse die Entwicklung der beteiligten Gewichte über sämtliche Lernphasen.

### G2 – Wiederverwendung

Prüfe, ob dieselben Verbindungen später für weitere Aufgaben relevant werden.

### G3 – Abschwächung

Untersuche Fälle, in denen bestehende Gewichte während einer verbesserten Abrufzuverlässigkeit abnehmen.

### G4 – Verstärkung

Untersuche ebenso Fälle mit steigenden Gewichten.

### G5 – Kombination

Prüfe, ob die gemeinsame Veränderung mehrerer Synapsen funktional notwendig ist.

### G6 – Kausale Gegenproben

Verwende vollständige Zustandskopien und den vorhandenen Forschungs-Interceptor.

### G7 – Verbotene Abkürzung

Keine automatische Stabilisierung, Sperrung oder Wiederherstellung erfolgreicher Gewichte.

**Eine gefestigte Fähigkeit muss nicht durch unveränderliche Synapsen repräsentiert sein.**

---

## 13. Phase H – Temporär nicht abrufbare Fähigkeiten

Untersuche Netze, die eine zuvor nachgewiesene Fähigkeit zeitweise nicht mehr zuverlässig ausführen können.

### H1 – Ausgangsnachweis

Die Fähigkeit muss zuvor unter klar definierten Prüfbedingungen verfügbar gewesen sein.

### H2 – Abrufverlust

Erfasse anschließend eine tatsächliche Beeinträchtigung unter vergleichbaren Bedingungen.

### H3 – Kontrollgruppen

Vergleiche:

- Erneute passende A-Erfahrungen.
- Unabhängige weitere Erfahrungen.
- Reinen Zeitablauf.
- Eine passende Kontrollgruppe ohne vorherigen A-Erwerb.

### H4 – Wiedererwerb

Untersuche, ob die früher trainierten Netze die Fähigkeit schneller zurückerlangen.

### H5 – Zustandsdiagnose

Prüfe, ob transiente oder synaptische Veränderungen den fehlenden Abruf erklären.

### H6 – Ergebnis

Unterscheide:

- Vorübergehende Abrufbeeinträchtigung.
- Nachweisbar erhaltene Vorstrukturierung.
- Erleichterten Wiedererwerb.
- Langfristige Beeinträchtigung.
- Ungeklärten Zustand.

Ein kurzfristiger Leistungseinbruch darf nicht automatisch als vollständiges Vergessen bezeichnet werden.

---

## 14. Phase I – Lernen mehrerer Fähigkeiten

Erweitere die Untersuchungen auf die aus B.5.11 bekannten Aufgaben A und B.

### I1 – A → B

Untersuche die Entwicklung von B und die gleichzeitige Veränderung des A-Abrufs.

### I2 – A → B → A

Untersuche den Wiedererwerb von A.

### I3 – A → B → gemischt

Untersuche, ob beide Fähigkeiten durch weitere Erfahrungen gemeinsam zuverlässiger werden.

### I4 – Teillernen

Prüfe, ob der Erwerb von B bereits die spätere Integration erleichtert, bevor B erstmals korrekt ausgeführt wird.

### I5 – Reorganisation

Untersuche, ob Veränderungen bereits für A verwendeter Verbindungen zur späteren Verbesserung des gesamten Repertoires beitragen.

### I6 – Kein Festplattenmodell

Das Netzwerk darf nicht für jede Aufgabe eine technisch vorgefertigte getrennte Speicherstruktur erhalten.

### I7 – Keine erzwungene gemeinsame Speicherung

Umgekehrt darf eine gemeinsame Repräsentation nicht künstlich vorgeschrieben werden.

Die funktionale Organisation muss sich aus dem neuronalen Lernprozess ergeben.

---

## 15. Phase J – Lernmechanismen und Bewertungsmethoden getrennt halten

Die neuronale Produktionslogik bleibt unverändert.

Insbesondere:

- Keine neue Eligibility-Regel.
- Keine neue synaptische Lernschrittweite.
- Keine automatische Gewichtssperre.
- Kein Rollback.
- Keine globalen Lernphasenkennungen.
- Keine semantischen Aufgabenbezeichnungen im Netz.
- Keine technische Kenntnis der richtigen Motorantwort.
- Kein globaler Reset zwischen Erfahrungen.

Die neue Forschung darf zusätzliche Beobachter, Prüfmethoden und diagnostische Zustandskopien verwenden.

Diese dürfen den natürlichen Lernprozess nicht verändern.

### Besonders wichtig: Exploration

Die in B.5.11 korrigierte Explorationsbewertung ist verbindlich.

Positive Rückmeldung darf nicht allein deshalb erfolgen, weil eine bestimmte Exploration geplant war.

Entscheidend ist die tatsächlich ausgeführte und von der Umwelt bewertete Handlung.

---

## 16. Phase K – Kontrollgruppen

Mindestens:

| Gruppe | Zweck |
|---|---|
| K0 | Unerfahrenes Netz |
| K1 | Erfahrungen mit A |
| K2 | Gleich viele unabhängige Erfahrungen |
| K3 | Gleiche Zeit ohne passende Erfahrungen |
| K4 | A-Vorerfahrung ohne bisherigen erfolgreichen Abruf |
| K5 | Gleiches anschließendes A-Training |
| K6 | Erster erfolgreicher Abruf |
| K7 | Weiteres A-Training nach erstem Erfolg |
| K8 | Pause nach erstem Erfolg |
| K9 | Andere Erfahrungen nach erstem Erfolg |
| K10 | Gemischte Erfahrungen nach erstem Erfolg |
| K11 | Vorübergehend beeinträchtigter A-Abruf |
| K12 | Wiedererwerb von A |
| K13 | A → B |
| K14 | A → B → A |
| K15 | A → B → gemischt |
| K16 | No-Learning |
| K17 | Diagnostische globale Schreibsperre |
| K18 | R0 |
| K19 | R2-50 |
| K20 | S4-d16 |
| K21 | Unveränderte B.5.11-Bewertung |
| K22 | Erweiterte Lernverlaufsbewertung |

Die zentrale Forschungsfrage ist nicht, welche Kontrollgruppe die höchste momentane Erfolgsquote erzielt.

Sie lautet, **welche Auswirkungen frühere Erfahrungen auf die spätere Entwicklung tatsächlich nutzbarer Fähigkeiten haben**.

---

## 17. Phase L – Bewertung über mehrere Lernstadien

Die bisherigen E1-/E2-Werte und B.5.11-Dimensionen V1–V10 bleiben erhalten.

Zusätzlich sind mindestens folgende Kennzahlen getrennt auszuweisen:

**L1 – Erster autonomer Erfolg**

Wann tritt erstmals eine richtige Antwort auf?

**L2 – Anfängliche Zuverlässigkeit**

Wie häufig ist die Antwort unmittelbar danach unter gültigen Bedingungen korrekt?

**L3 – Weitere Lernentwicklung**

Wie verändert sich die Fähigkeit durch zusätzliche Erfahrungen?

**L4 – Latenter Lernvorteil**

Wie stark unterscheiden sich zuvor trainierte, aber noch erfolglose Netze von kontrollierten unerfahrenen Netzen beim anschließenden Lernen?

**L5 – Kontextabhängige Verfügbarkeit**

Unter welchen gültigen Bedingungen kann die Fähigkeit abgerufen werden?

**L6 – Verfestigung**

Wie verändert sich die zuverlässige Abrufbarkeit nach weiteren Erfahrungen?

**L7 – Retention**

Wie bleibt die Fähigkeit über Zeit oder konkurrierende Erfahrungen verfügbar?

**L8 – Wiedererwerb**

Wie viele weitere Erfahrungen benötigt ein vorübergehend beeinträchtigtes Netz gegenüber einer passenden Kontrollgruppe?

**L9 – Neuronale Reorganisation**

Welche Veränderungen tragen kausal zur späteren Verbesserung bei?

**L10 – Lernressourcen**

Welche neuronalen und technischen Ressourcen werden für die beobachtete Entwicklung benötigt?

### Verbindliche Auswertung

Keine dieser Kennzahlen darf ungeprüft als allgemeiner Lernscore verwendet werden.

Insbesondere dürfen nicht gleichgesetzt werden:

- Fehlende Antwort und fehlende Vorstrukturierung.
- Erste richtige Antwort und stabile Beherrschung.
- Unveränderte Gewichte und Fähigkeitserhalt.
- Veränderte Gewichte und erfolgreicher Erwerb.
- Kurzfristiger Verlust und langfristiges Vergessen.

---

## 18. Phase M – Kausale Untersuchungen

### M1 – Frühe Erfahrung

Vergleiche mit und ohne vorherige A-Erfahrungen.

### M2 – Einzelne Gewichtsänderungen

Prüfe ausgewählte tatsächliche Schreiboperationen mit W0/W1/W2.

### M3 – Kombinierte Änderungen

Untersuche die gemeinsame Wirkung mehrerer Veränderungen.

### M4 – Transiente Zustände

Verwende vollständige Zustandskopien zur Unterscheidung von Membran-, Eligibility- und Gewichtseinflüssen.

### M5 – Weitere natürliche Erfahrungen

Die Fortsetzung muss tatsächliche neuronale Antworten und tatsächlich resultierende Rückmeldungen verwenden.

### M6 – Spätere Fähigkeiten

Untersuche nicht nur die nächste Antwort, sondern die Entwicklung über weitere Erfahrungen.

### M7 – Keine nachträgliche Optimierung

Eine Veränderung darf nicht allein deshalb als lernförderlich ausgewählt werden, weil ihr späterer Erfolg aus den Forschungsdaten bereits bekannt ist.

### M8 – Vorsicht bei Kausalität

Ein beschleunigter späterer Erwerb nach Vortraining zeigt einen funktionalen Einfluss der Vorerfahrung.

Welche konkrete neuronale Veränderung dafür verantwortlich ist, muss gesondert kausal untersucht werden.

---

## 19. Phase N – Versuchsplanung und Statistik

### Discovery

Mindestens:

- 64 Seeds je zentraler Zufallsstromfamilie und Zuordnung.
- Drei unabhängige Stromfamilien.
- Normale und inverse Zuordnung, soweit anwendbar.
- Vorab festgelegte Trainingsepisoden und Prüfzeitpunkte.

### Bestätigung

Mindestens 64 weitere, bisher unbenutzte Seeds je zentraler Kombination.

### Vergleichbarkeit

Die Versuche müssen kontrollieren:

- Identische neuronale Initialisierung innerhalb gepaarter Vergleiche.
- Gleiche weitere sensorische Lernpläne.
- Gleiche äußere Aufgabenregeln.
- Vergleichbare Erfahrungsanzahl.
- Vergleichbare Simulationszeit.
- Kontrollierte Exploration.
- Vollständig dokumentierte Rückmeldungen.

Dabei müssen unterschiedliche tatsächlich ausgeführte Handlungen auch unterschiedliche natürliche Rückmeldungen erhalten dürfen.

### Statistik

Erforderlich:

- Gepaarte Vergleiche.
- Seedbasierte Clusterung.
- 95-%-Bootstrap-Konfidenzintervalle.
- Separate Auswertung einzelner Lernstadien.
- Separate Auswertung von Discovery und Bestätigung.
- Effektstärken und Fallzahlen.
- Lernkurven und nicht nur Endwerte.
- Auswertung erfolgloser und erfolgreicher Netze.
- Dokumentation nicht erreichter Lernstadien.

Keine nachträgliche Änderung von Erfolgsschwellen aufgrund günstiger Bestätigungsdaten.

---

## 20. Phase O – Persistenz und Determinismus

Erforderlich:

- Identische vollständige Zustandskopien.
- Gespeicherte neuronale Gewichte.
- Transiente Membranzustände.
- Eligibility.
- Refraktärzustände.
- Recovery.
- Ausstehende zeitliche Ereignisse.
- Zufallszustände.
- Deterministische Fortsetzung nach Laden.
- Verhaltenstreue bei deaktivierten Forschungsbeobachtern.

S4 bleibt standardmäßig deaktiviert.

Ein Persistenztest mit tatsächlich ausstehendem S4-Ereignis muss erhalten bleiben.

Die neuen Bewertungsverfahren dürfen keine zusätzlichen persistenten Lerninformationen in EVE einführen.

---

## 21. Regressionstests

Alle nach der B.5.11-Korrektur vorhandenen Tests müssen bestehen bleiben.

Neue Tests mindestens für:

- Lernen vor der ersten richtigen Antwort.
- Veränderte Gewichte ohne nachgewiesenes Lernen.
- Funktionaler Vorteil früher Erfahrungen.
- Unerfahrene Kontrollgruppe.
- Aufgabenunabhängige Erfahrungskontrolle.
- Zeitkontrolle.
- Gleiche weitere Lernerfahrungen.
- Erster autonomer Erfolg.
- Wiederholter erfolgreicher Abruf.
- Zuverlässige Fähigkeit.
- Verfestigung durch weitere Erfahrungen.
- Pause ohne Lernerfahrung.
- Weitere synaptische Reorganisation.
- Vorübergehende Abrufbeeinträchtigung.
- Erleichterter Wiedererwerb.
- A → B → A.
- A → B → gemischt.
- Tatsächlich ausgeführte Exploration.
- Diagnostische Isolation des Trainingsoriginals.
- W0/W1/W2.
- R0/R2/S4.
- Geschützte Exploration.
- Persistenz.
- Deterministische Fortsetzung.
- Gültige neuronale Zustände.

Die vorhandenen historischen Regressionen dürfen nicht abgeschwächt oder entfernt werden.

---

## 22. Wissenschaftliche Erfolgskriterien

B.5.12 gilt als erfolgreich abgeschlossen, wenn:

1. Die bisherigen Bewertungsmethoden um die zeitliche Entwicklung von Fähigkeiten erweitert wurden.
2. Der vollständige Lernverlauf erfasst wird.
3. Erster Erfolg und zuverlässige Beherrschung getrennt gemessen werden.
4. Frühere Erfahrungen kausal auf ihren Einfluss auf späteres Lernen untersucht wurden.
5. Netze ohne bisherigen erfolgreichen Abruf ausdrücklich berücksichtigt werden.
6. Teillernen und latentes Lernen nicht allein aus Gewichtsänderungen abgeleitet werden.
7. Zusätzliche Erfahrungen und reine Zeitkontrollen unterschieden werden.
8. Verfestigung unabhängig von unveränderten Gewichten untersucht wird.
9. Vorübergehende Abrufbeeinträchtigung und Wiedererwerb geprüft werden.
10. Mehrfachlernen und gemeinsame Reorganisation berücksichtigt werden.
11. Unabhängige Bestätigung vorliegt.
12. Die Grenzen der gewonnenen Erkenntnisse dokumentiert sind.
13. Persistenz, Determinismus und sämtliche Regressionen bestehen bleiben.

**Ein wissenschaftlich negativer Befund gilt ausdrücklich als gültiges Forschungsergebnis.**

Es ist nicht erforderlich, latentes Lernen oder eine bestimmte Form der Verfestigung nachzuweisen.

Entscheidend ist, dass unsere Prüfmethoden solche Phänomene zuverlässig untersuchen können, ohne sie vorauszusetzen.

---

## 23. Lieferumfang

Nova-EVE erstellt:

1. Exakte Reproduktion des korrigierten B.5.11-Standes.
2. Dokumentation der erweiterten Lernbegriffe.
3. Neue Lernverlaufs- und Kontrollgruppen.
4. Untersuchungen zu Teillernen.
5. Untersuchungen zu latentem Lernen.
6. Vortrainings- und Weiterlernvergleiche.
7. Untersuchungen zum ersten autonomen Abruf.
8. Untersuchungen zur zuverlässigen Beherrschung.
9. Untersuchungen zur Verfestigung.
10. Zeit- und Erfahrungskontrollen.
11. Retentions- und Wiedererwerbsversuche.
12. A-/B-Mehrfachlernversuche.
13. Kausale Einzel- und Kombinationsinterventionen.
14. Getrennte Lernstadien-Auswertungen.
15. Vollständige Lernkurven.
16. Unabhängige Bestätigungsdaten.
17. Aktualisierte Regressionstests.
18. Persistenz- und Ressourcenprüfung.
19. Forschungsbericht B.5.12.
20. Technische Kausalanalyse der Lernentwicklung.
21. Methodische Bewertung der bisherigen Lernkriterien.
22. Architekturentscheidung.
23. Entscheidungsvorlage B.5.13.
24. Explizite Merge-Empfehlung.

---

## 24. Abschlussfragen

**A.** Kann EVE durch Erfahrungen bereits funktional relevante Vorstrukturierung erwerben, bevor die erste erfolgreiche Motorantwort entsteht?

**B.** Lernen zuvor trainierte, aber noch erfolglose Netze nachweislich schneller als unerfahrene Netze?

**C.** Ist dieser Vorteil aufgabenspezifisch oder nur Folge allgemeiner neuronaler Aktivität?

**D.** Welche neuronalen Veränderungen sind dafür kausal verantwortlich?

**E.** Wie unterscheiden sich erster erfolgreicher Abruf und zuverlässige Beherrschung?

**F.** Welche Rolle spielen weitere Erfahrungen bei der Verfestigung?

**G.** Können bereits beteiligte Synapsen während der Verfestigung erneut verändert werden?

**H.** Können Synapsen abgeschwächt werden, während die Zuverlässigkeit einer Fähigkeit zunimmt?

**I.** Können sich Abrufmöglichkeiten ohne neue Lernerfahrungen verändern, und welche vorhandenen lokalen Ereignisse erklären dies?

**J.** Welche vorübergehend nicht abrufbaren Fähigkeiten besitzen weiterhin eine funktional nachweisbare Vorstrukturierung?

**K.** Unter welchen Bedingungen ist Wiedererwerb schneller als erstmaliger Erwerb?

**L.** Welche Rolle spielt die gemeinsame neuronale Reorganisation beim Erwerb zusätzlicher Fähigkeiten?

**M.** Welche unserer bisherigen Bewertungsgrößen übersehen relevante Teile des Lernverlaufs?

**N.** Welche neuen Bewertungsmethoden sollten dauerhaft Bestandteil der EVE-Forschung werden?

**O.** Welche Grenzen der vorhandenen neuronalen Architektur werden dadurch sichtbar?

**P.** Was folgt daraus für die nächste Forschungsstufe?

---

## 25. Merge-Entscheidung

Der Forschungsbericht muss eine eindeutige Empfehlung enthalten:

**B.5.12 nach `main` mergen: Ja oder Nein.**

Ein Merge ist nur zu empfehlen, wenn:

- Die neue Bewertungsinfrastruktur technisch valide ist.
- Historische Forschungsreferenzen reproduziert werden.
- Alle Regressionstests bestehen.
- Diagnostische Tests das ursprüngliche Lernen nicht verändern.
- Die neuen Lernstadien methodisch sauber getrennt werden.
- Keine unzulässigen Zielinformationen in die neuronale Architektur gelangen.
- Persistenz und Determinismus erhalten bleiben.
- Keine neue neuronale Produktionsregel automatisch aktiviert wird.

Eine fehlende Bestätigung von latentem Lernen oder Verfestigung ist kein Merge-Blocker.

---

## 26. Verbindliche Schlussleitlinie

**Wir dürfen neuronales Lernen nicht ausschließlich daran messen, ob EVE zum Zeitpunkt unserer Prüfung die erwartete Antwort erzeugt.**

Eine Fähigkeit kann sich allmählich entwickeln.

Frühere Erfahrungen können bereits neuronale Strukturen verändert haben, bevor deren funktionaler Nutzen unmittelbar sichtbar wird.

Zusätzliche Erfahrungen können notwendig sein, damit eine vorhandene Vorstrukturierung zuverlässig abrufbar wird.

Gleichzeitig gilt:

Nicht jede neuronale Veränderung ist Lernen.

Nicht jede vermeintliche Vorstrukturierung besitzt einen nachweisbaren Nutzen.

Und nicht jede vorübergehend fehlende Antwort belegt, dass eine Information neuronal erhalten geblieben ist.

Diese Unterschiede müssen anhand kontrollierter weiterer Erfahrungen und kausaler Gegenproben untersucht werden.

**Das Ziel von B.5.12 ist nicht, EVE schneller zur richtigen Antwort zu bringen.**

**Das Ziel ist, herauszufinden, wie sich eine neuronale Fähigkeit tatsächlich entwickelt – auch während jener Phasen, in denen wir noch keine erfolgreiche Handlung beobachten können.**

Erst wenn unsere Bewertungsmethoden diese Entwicklung erfassen können, ist es wissenschaftlich sinnvoll, weitere neuronale Lernmechanismen zu entwickeln.
