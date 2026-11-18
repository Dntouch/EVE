# EVE – Forschungsstufe A.2
## Kausalität neuronaler Handlungen und motorischer Antwortabschlüsse

**Auftrag an:** Nova-EVE  
**Projekt:** EVE – Emergent Virtual Evolution  
**Repository:** `Dntouch/EVE`  
**Ausgangsbasis:** `codex/research-stage-a1-sensorimotor-foundations`  
**Status:** Forschungs- und Entwicklungsauftrag  
**Implementierung:** C++17

---

# 1. Ausgangssituation

Forschungsstufe A.1 hat vier grundlegende Mechanismen erfolgreich als technische Grundlage bereitgestellt:

1. Angeborene neuronale Grundverschaltung.
2. Explorative neuronale Aktivität.
3. Motorische Grundschaltungen.
4. Sensorische Eigen- und Umweltrückmeldung.

Das Netzwerk kann inzwischen durch neuronale Feuerereignisse motorische Ausgänge aktivieren und seine eigene Ausgangsaktivität als neuen sensorischen Reiz empfangen.

Allerdings zeigen die Messergebnisse ein grundlegendes Problem.

In den untersuchten Konfigurationen war die Abschlussrate nicht zuverlässig von einer vorhandenen motorischen Grundschaltung abhängig.

Insbesondere ergaben sich in Forschungsphase III:

| Variante | Vollständige Antworten |
|---|---:|
| M0 – Keine Motorschaltung | 3 von 6 |
| M1 – Direkter Abschlussweg | 1 von 6 |
| M2 – Motorische Relaisgruppe | 1 von 6 |
| M3 – Aktivitätsabhängiger Akkumulator | 1 von 6 |

Die Versuchsbedingungen ermöglichten explorative Aktivierungen im gesamten neuronalen Netzwerk.

Dadurch konnten auch motorische Kontakte und der Abschlusskontakt direkt durch Exploration aktiviert werden.

Ein solcher Abschluss ist technisch neuronal, stellt aber noch keinen Nachweis einer funktionierenden motorischen Handlungsorganisation dar.

**Forschungsstufe A.2 soll untersuchen, ob die vorhandenen motorischen Grundschaltungen einen Abschluss tatsächlich infolge vorausgegangener motorischer Aktivität erzeugen.**

---

# 2. Zentrale Forschungsfrage

**Kann EVE eine motorische Handlung durch ihre neuronale Verschaltung ausführen und abschließen, wenn die Exploration ausschließlich allgemeine Verarbeitungsneuronen aktiviert und motorische Kontakte nur über normale neuronale Signalwege erreichbar sind?**

Dabei sollen drei Bedingungen erfüllt sein:

1. Exploration aktiviert keine motorischen Kontakte unmittelbar.
2. Ein Abschluss entsteht gegebenenfalls aus regulärer neuronaler Verarbeitung und der motorischen Grundschaltung.
3. Das Verhalten wird mit identischen Ausgangsbedingungen ohne motorische Grundschaltung verglichen.

Der Schwerpunkt liegt auf der tatsächlichen Ursache eines Abschlussereignisses.

Es werden noch keine Lern- oder Belohnungsmechanismen eingeführt.

# 3. Grundlegende Vorgaben

Die bestehende Architektur bleibt erhalten.

- BIOS: technische Ereignisverarbeitung und Beobachtung.
- Stammhirn: angeborene Verschaltung und Exploration.
- Neuronales Netzwerk: eigenständige Verarbeitung und Feuerereignisse.

Weiterhin gelten:

- Keine vorgegebenen Antworten.
- Keine semantischen Neuronen.
- Keine direkte Eingabe-Ausgabe-Kopie.
- Keine künstliche Aktivierung des Abschlusskontakts als Ersatz für neuronale Verarbeitung.
- Keine globale neuronale Uhr.
- Keine Zeitstempel pro Synapse.
- Keine neuen synaptischen Lernregeln.
- Keine neuen Hemmungs- oder Regulationsmechanismen.
- Keine zusätzlichen neuronalen Frameworks.

Alle bereits implementierten Mechanismen sollen konfigurierbar bleiben.

# 4. Forschungsphase I – Explorationsbereiche trennen

## 4.1 Ziel

Verhindere, dass explorative Aktivität unmittelbar diejenigen Neuronen aktiviert, deren Feuern als technische Handlung oder Antwortabschluss ausgewertet wird.

## 4.2 Erlaubte Explorationsziele

Exploration darf ausschließlich Neuronen aktivieren, die aktuell keine reservierte sensorische, motorische oder angeborene Grundschaltungsfunktion erfüllen.

Ausgeschlossen sind insbesondere:

- Sensorische Eingangskontakte 0 bis 7.
- Der Antwortreizkontakt 8.
- Die für Rückmeldungen reservierten Neuronen 9 bis 17.
- Sämtliche motorischen Datenkontakte.
- Der motorische Abschlusskontakt.
- Sämtliche Neuronen der motorischen Abschlussgruppe beziehungsweise des Akkumulators.
- Weitere technisch reservierte Kontakte der jeweiligen Versuchskonfiguration.

Die zulässigen Explorationsneuronen werden aus der tatsächlichen Konfiguration abgeleitet.

Es dürfen keine festen ID-Bereiche angenommen werden, die bei zukünftigen Netzgrößen nicht mehr stimmen.

## 4.3 Wichtige Unterscheidung

Die ausgeschlossenen Neuronen bleiben regulär neuronal aktivierbar.

Sie dürfen weiterhin:

- Synaptische Impulse empfangen.
- Ihr Membranpotenzial verändern.
- Feuern.
- Synapsen verstärken, sofern Lernen aktiviert ist.
- Neue Synapsen bilden, sofern Wachstum aktiviert ist.

**Nur ihre unmittelbare Auswahl als Explorationsziel wird ausgeschlossen.**

Das Netzwerk darf sie weiterhin über seine eigenen Verbindungen aktivieren.

## 4.4 Versuchsvarianten

**E0 – Keine Exploration**

Referenz ohne explorative Aktivierung.

**E4-alt – Bisherige verteilte Exploration**

Reproduziert das Verhalten aus Forschungsstufe A.1.

**E4-neu – Verteilte Exploration nur allgemeiner Verarbeitungsneuronen**

Explorationsziele werden ausschließlich aus der zulässigen Menge ausgewählt.

Die Zahl der Aktivierungen bleibt konfigurierbar.

Als Startwerte sollen 8, 16 und 32 Aktivierungen untersucht werden.

Die Auswahl erfolgt weiterhin reproduzierbar über einen eigenen Zufallszustand.

## 4.5 Zu messen

- Anzahl zulässiger Explorationsneuronen.
- Anzahl ausgelöster Explorationsereignisse.
- Anzahl dadurch erreichter motorischer Kontakte.
- Anzahl direkt explorativ aktivierter motorischer Kontakte.
- Anzahl vollständiger Antworten.
- Anzahl Abschlüsse ohne vorherige Datenmotoraktivität.

Bei E4-neu muss die Zahl der unmittelbar explorativ aktivierten Motor- und Abschlusskontakte exakt null betragen.

Ein Verstoß gilt als Fehler des Versuchsaufbaus.

# 5. Forschungsphase II – Motorische Abschlusskausalität

## 5.1 Ziel

Untersuche, ob die vorhandenen motorischen Grundschaltungen tatsächlich dazu beitragen, eine motorische Aktivität neuronal abzuschließen.

Dabei werden die bestehenden Motorschaltungsvarianten unverändert verwendet.

## 5.2 Zu vergleichende Varianten

| Variante | Organisation |
|---|---|
| M0 | Keine zusätzliche Motorschaltung |
| M1 | Direkter neuronaler Abschlussweg |
| M2 | Motorische Relaisgruppe |
| M3 | Aktivitätsabhängiger Akkumulator |

Alle Varianten verwenden:

- Dasselbe neuronale Grundmodell.
- Dieselbe angeborene Grundverschaltung.
- Dieselben regulären Startsynapsen.
- Dieselben Startgewichtungen.
- Dieselben sensorischen Eingaben.
- Die eingeschränkte Exploration E4-neu.

Die jeweils hinzugefügte motorische Grundschaltung bildet die untersuchte unabhängige Variable.

## 5.3 Ereignisfolge

Für jede Antwortphase werden die tatsächlich eintretenden motorischen Ereignisse in ihrer Reihenfolge aufgezeichnet.

Beispiel:

1. Allgemeines Verarbeitungsneuron feuert.
2. Motorisches Datenneuron feuert.
3. Ein Relaisneuron erhält einen Impuls.
4. Das Relaisneuron feuert.
5. Der Abschlusskontakt empfängt einen Impuls.
6. Der Abschlusskontakt feuert.

Ein solcher Ablauf wäre mit einem funktionalen Abschlussweg vereinbar.

Erst durch Vergleich mit Kontrollbedingungen kann beurteilt werden, ob die untersuchte Grundschaltung für diesen Ablauf tatsächlich erforderlich war.

## 5.4 Unterscheidung der Abschlussereignisse

Dokumentiere ausdrücklich:

**Abschluss ohne Datenmotoraktivität**

Der Abschlusskontakt feuert, obwohl während der Antwortphase kein Datenkanal aktiviert wurde.

Das kann technisch eine gültige Nullbyte-Antwort sein, ist aber kein Beleg für einen Abschluss infolge einer vorausgegangenen Datenmotoraktivität.

**Abschluss nach Datenmotoraktivität**

Mindestens ein motorischer Datenkontakt wurde aktiviert, bevor der Abschlusskontakt feuerte.

Dies ist ein möglicher Hinweis auf einen motorisch organisierten Abschluss.

**Abschluss über nachgewiesenen Schaltungsweg**

Die Ereignisdiagnose zeigt zusätzlich die tatsächliche Signalübertragung über die betreffende motorische Grundschaltung.

**Technischer Abbruch**

Die Ereignisverarbeitung endet aufgrund einer Sicherheits- oder Ressourcenbegrenzung.

Dieser Fall ist kein neuronaler Abschluss.

## 5.5 Zu messen

Erfasse:

- Anzahl motorischer Datenereignisse.
- Anzahl neuronaler Abschlussereignisse.
- Anzahl Abschlüsse mit und ohne vorausgegangene Datenaktivität.
- Reihenfolge der motorischen Ereignisse.
- Anzahl verarbeiteter Ereignisse zwischen erster Datenaktivität und Abschluss.
- Beteiligte Motorschaltungsneuronen.
- Erregungszustand und Refraktärzustand des Abschlussneurons beim Empfang relevanter Impulse.
- Anzahl Antwortphasen ohne Abschluss.
- Maximale Ereigniswarteschlange.

Die Daten sollen ermöglichen, einen tatsächlichen Schaltungsablauf von einem zufälligen Treffer des Abschlusskontakts zu unterscheiden.

# 6. Forschungsphase III – Kontrollierte Vergleichsversuche

## 6.1 Ziel

Ermittle, ob die motorischen Grundschaltungen die Wahrscheinlichkeit eines Abschlusses infolge vorausgegangener Datenmotoraktivität erhöhen.

## 6.2 Identische Ausgangsbedingungen

Verwende mindestens die etablierten Seeds:

`20261117`, `7`, `42`, `99`, `2026`, `314159`

Ergänze weitere reproduzierbare Seeds, damit die Auswertung nicht von wenigen zufälligen Verläufen dominiert wird.

Für jede Vergleichsgruppe gelten:

- Identische ursprüngliche Netzwerkinitialisierung.
- Identische angeborene Grundverschaltung vor Ergänzung der motorischen Schaltung.
- Identische sensorische Eingaben.
- Identische Explorationsauswahl.
- Identische Regulationsparameter.
- Identische Ereignislimits.

Die Explorationsfolge muss unabhängig von der hinzugefügten Motorschaltung erzeugt werden.

Hierfür sind getrennte Zufallszustände zu verwenden.

## 6.3 Kontrollgruppen

**K0 – Ohne motorische Grundschaltung**

Untersucht, wie häufig bereits die reguläre Vernetzung zu Abschlüssen führt.

**K1 – Mit motorischer Grundschaltung**

Untersucht das zusätzliche Verhalten durch M1, M2 oder M3.

**K2 – Grundschaltung gezielt unterbrochen**

Verwendet dieselbe Grundkonfiguration wie K1, deaktiviert jedoch die zu prüfende motorische Verbindung beziehungsweise Schaltungsstrecke.

Damit kann untersucht werden, ob ein beobachteter Abschluss tatsächlich von dieser Strecke abhängig war.

**K3 – Kontrollverschaltung**

Optional kann zusätzlich eine gleich große, aber nicht auf den Abschluss gerichtete Vergleichsverschaltung untersucht werden.

Sie hilft zu unterscheiden, ob Effekte lediglich durch zusätzliche positive Synapsen entstehen.

Alle Kontrolländerungen müssen technisch dokumentiert sein.

## 6.4 Zentrale Vergleichsmaße

Ermittle mindestens:

- Anteil der Antwortphasen mit Datenmotoraktivität.
- Anteil vollständiger neuronaler Antworten.
- Anteil der Abschlüsse nach vorausgegangener Datenmotoraktivität.
- Anteil der Abschlüsse ohne Datenmotoraktivität.
- Anteil tatsächlich nachvollziehbarer Abschlüsse über die betreffende Schaltung.
- Unterschiede zwischen aktiver und unterbrochener Grundschaltung.

Besonders wichtig ist die bedingte Abschlusswahrscheinlichkeit:

`P(Abschluss | vorausgegangene Datenmotoraktivität)`

Diese ist zusätzlich zu betrachten zur allgemeinen Abschlussrate:

`P(Abschluss)`

Ein Abschluss ohne Datenmotoraktivität darf die Bewertung einer motorabhängigen Grundschaltung nicht künstlich verbessern.

## 6.5 Abbruch und Fehlschläge

Versuche dürfen nicht verworfen werden, nur weil keine motorische Aktivität entsteht.

Fehlende Datenmotoraktivität ist ein eigenes Ergebnis und muss von einem fehlenden Abschluss nach erfolgter Datenaktivität getrennt bleiben.

Auch ein nicht abgeschlossener Versuch ist als regulärer Forschungsbefund zu dokumentieren.

# 7. Forschungsphase IV – Wiederholte Handlungsabläufe

## 7.1 Ziel

Untersuche, ob die motorischen Grundschaltungen nicht nur einmalig, sondern auch bei wiederholten Antwortaufforderungen funktionieren.

## 7.2 Ablauf

1. Initialisiere das Netzwerk.
2. Präsentiere einen sensorischen Reiz.
3. Starte eine Antwortphase.
4. Führe die eingeschränkte Exploration aus.
5. Beobachte Datenmotoraktivität und Abschluss.
6. Beende die Antwortphase regulär.
7. Wiederhole den Ablauf mit demselben Netzwerkzustand beziehungsweise dessen dokumentierter Weiterentwicklung.

Untersuche jeweils getrennt:

- Wiederholungen ab identisch wiederhergestelltem Zustand.
- Wiederholungen mit fortgeführtem neuronalen Zustand.

Dadurch wird erkennbar, ob Unterschiede durch die Versuchskonfiguration oder durch die fortlaufende Veränderung neuronaler Zustände entstehen.

## 7.3 Untersuchungen

- Bleibt die Abschlusswahrscheinlichkeit konstant?
- Entstehen wiederkehrende motorische Aktivitätsfolgen?
- Häufen sich unmittelbare oder verfrühte Abschlüsse?
- Verändert sich das Verhalten durch unterschiedliche Eingaben?
- Bleibt das Netzwerk aktiv und stabil?
- Welche Auswirkungen hat die Refraktärphase?

Die vorhandene Refraktärregel darf nicht stillschweigend verändert werden.

# 8. Forschungsphase V – Überprüfung der sensorischen Rückkopplung

Die Eigenrückmeldung aus Forschungsstufe A.1 bleibt grundsätzlich erhalten.

Für die primären Kausalitätsversuche wird sie jedoch deaktiviert, damit sie die untersuchten motorischen Abläufe nicht beeinflusst.

Erst nach Abschluss der motorischen Vergleichsversuche wird sie wieder zugeschaltet.

Untersuche anschließend:

- Werden tatsächlich abgeschlossene Antworten korrekt zurückgemeldet?
- Bleibt die ursprüngliche motorische Ereignisfolge rekonstruierbar?
- Welche zusätzliche neuronale Aktivität entsteht durch Eigenrückmeldung?
- Verändert die Rückmeldung die nächste Antwortphase?
- Kommt es zu selbstverstärkenden Rückkopplungen?

Eigenrückmeldung und beobachtete Umweltwirkung müssen weiterhin getrennt bleiben.

Es wird noch keine Bewertung der Handlung eingeführt.

# 9. Lern- und Wachstumsregeln

Die primären Untersuchungen erfolgen zunächst mit deaktiviertem Lernen und Wachstum.

Damit wird die Wirkung der angeborenen motorischen Organisation isoliert.

Nach der Kausalitätsprüfung dürfen zusätzliche, separat dokumentierte Versuche mit den bereits vorhandenen Lern- und Wachstumsregeln erfolgen.

Dabei gelten weiterhin die bisherigen Grenzen:

- Keine neuen Verstärkungsmechanismen.
- Keine synaptische Abschwächung.
- Keine zusätzlichen Hemmungsmechanismen.
- Keine STDP.
- Keine Belohnungs- oder Bestrafungssignale.
- Keine neuen lokalen Ereignisgedächtnisse.

Die vorhandenen Regulationsvarianten C5 und C5+A1 können als experimentelle Parameter verwendet werden.

Sie sind nicht als endgültige neuronale Architektur festgelegt.

# 10. Technische Diagnose

Die vorhandene Ereignisprotokollierung soll gezielt um Informationen erweitert werden, mit denen sich motorische Ereignisfolgen nachvollziehen lassen.

Insbesondere relevant sind:

- Neuron, das ein Feuerereignis ausgelöst hat.
- Auslösende synaptische Verbindung, soweit vorhanden.
- Motorischer Kanal beziehungsweise Abschlusskontakt.
- Zugehörige Motorschaltungsvariante.
- Ereignisreihenfolge.
- Membranpotenzial vor und nach relevanten Eingängen.
- Refraktärzustand.
- Tatsächlich weitergeleitete synaptische Impulse.

Für die Diagnose darf eine technische Ereigniszuordnung im Versuchsprotokoll verwendet werden.

**Es dürfen keine zusätzlichen Zeitstempel oder Ereignisverläufe dauerhaft in den Synapsen gespeichert werden.**

Die Diagnose gehört zur Forschungsinstrumentierung und darf das neuronale Verhalten nicht verändern.

# 11. Automatisierte Tests

Ergänze die vorhandene Testsuite um mindestens folgende Prüfungen:

1. Explorationsziele enthalten keine reservierten sensorischen Kontakte.
2. Explorationsziele enthalten keine motorischen Datenkontakte.
3. Der Abschlusskontakt wird niemals unmittelbar explorativ aktiviert.
4. Motorische Relais- und Akkumulatorneuronen sind von unmittelbarer Exploration ausgeschlossen.
5. Die reguläre synaptische Aktivierung dieser Neuronen bleibt möglich.
6. Identische Explorationsfolgen sind bei gleichen Seeds reproduzierbar.
7. Die Explorationsfolge bleibt unabhängig von der aktivierten Motorschaltungsvariante.
8. M0 erzeugt keine zusätzlich angelegte Motorschaltung.
9. M1 bis M3 erzeugen die erwarteten neuronalen Verbindungen.
10. Das gezielte Unterbrechen einer Motorschaltung deaktiviert den betreffenden Signalweg.
11. Ein Abschluss ohne Datenmotoraktivität wird korrekt gesondert ausgewiesen.
12. Ein Abschluss nach Datenmotoraktivität wird korrekt erkannt.
13. Die Ereignisdiagnose kann tatsächliche Schaltungswege nachvollziehen.
14. Ein technischer Abbruch gilt nicht als neuronaler Abschluss.
15. Die Nullbyte-Ausgabe bleibt von fehlender Antwort unterscheidbar.
16. Die bestehende Persistenz und Reproduzierbarkeit bleiben erhalten.
17. Die bestehenden automatisierten Tests bestehen weiterhin.

# 12. Performance und Speicherverbrauch

Die neuen Mechanismen sollen möglichst wenig zusätzliche Verwaltungsdaten benötigen.

Ermittle:

- Speicherbedarf der eingeschränkten Explorationsauswahl.
- Aufwand zur Ermittlung reservierter Neuronen.
- Aufwand für die unterschiedlichen Motorschaltungen.
- Zusätzlichen Aufwand der Ereignisdiagnose.
- Synaptische Ereignisse pro Sekunde.
- Rechenzeit je Antwortphase.
- Größe der Ereigniswarteschlange.

Performance-Messungen mit aktivierter Detaildiagnose müssen getrennt von Messungen ohne Diagnose erfolgen.

Keine neue GPU-Implementierung.

# 13. Dokumentation und Lieferumfang

Erstelle im Repository:

- Forschungsbericht A.2 als Markdown.
- Reproduzierbare Versuchsskripte.
- Maschinenlesbare Rohdaten.
- Konfigurations- und Parameterübersicht.
- Vergleichstabellen für M0 bis M3.
- Getrennte Auswertung der Explorationsvarianten.
- Diagnosebeispiele vollständiger neuronaler Ereignisfolgen.
- Ergänzte automatisierte Tests.
- Entscheidungsvorlage für den Übergang zu Forschungsstufe B.

Bestehende Forschungsberichte und Messdaten müssen erhalten bleiben.

Vorhandene lokale Änderungen dürfen nicht überschrieben werden.

# 14. Abschlusskriterien

Forschungsstufe A.2 ist abgeschlossen, wenn folgende Fragen anhand reproduzierbarer Daten beantwortet werden können:

1. Kann neuronale Exploration ohne direkte Motoraktivierung weiterhin motorische Ausgänge erreichen?
2. Können motorische Grundschaltungen einen Abschluss nach vorausgegangener Datenmotoraktivität erzeugen?
3. Ist dieser Abschluss nachweisbar von der betreffenden Grundschaltung abhängig?
4. Wie unterscheiden sich M0, M1, M2 und M3 bei identischen Ausgangsbedingungen?
5. Welche Rolle spielen zufällige Abschlüsse?
6. Ist die Handlungsorganisation über wiederholte Versuche stabil?
7. Funktioniert die sensorische Eigenrückmeldung weiterhin?
8. Welche Variante eignet sich am besten als Grundlage für späteres rückmeldungsabhängiges Lernen?

Eine hohe Abschlussrate allein ist kein hinreichendes Erfolgskriterium.

Es muss erkennbar sein, **ob der Abschluss tatsächlich aus der vorangegangenen motorischen Aktivität entstanden ist**.

Die abschließende Entscheidung über die weitere Stammhirnarchitektur erfolgt erst nach gemeinsamer Auswertung.

---

## Zentraler Forschungsgrundsatz

**Eine neuronale Handlung ist nicht dadurch funktional organisiert, dass zufällig motorische Neuronen feuern. Entscheidend ist, ob die neuronale Verschaltung einen nachvollziehbaren Zusammenhang zwischen Aktivität, Handlung und Abschluss herstellt.**

Forschungsstufe A.2 soll diesen Zusammenhang erstmals gezielt und reproduzierbar untersuchen.

Erst danach folgt Forschungsstufe B mit der lokalen neuronalen Ereigniszuordnung als Grundlage für rückmeldungsabhängiges Lernen.
