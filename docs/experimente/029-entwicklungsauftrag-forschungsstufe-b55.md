# Entwicklungsauftrag 029 – Forschungsstufe B.5.5

## Lokale subthreshold-Membrandynamik und ereignisgesteuerte Relaxation

**Projekt:** EVE – Emergent Virtual Evolution  
**Technologie:** C++17  
**Ausgangsbasis:** Konsolidierter Forschungsstand B.5.4  
**Forschungsbranch:** `codex/research-stage-b55-subthreshold-membrane-relaxation`  
**Status:** Kausale Grundlagenforschung mit bedingter lokaler Erweiterung

---

## 1. Ausgangslage

Die Forschungsstufen B.5.2 bis B.5.4 haben gezeigt, dass die momentane neuronale Membranlage einen wesentlichen Einfluss darauf besitzt, ob EVE bereits erworbene Fähigkeiten autonom abrufen kann.

B.5.3 identifizierte die Membranpotenziale der Entscheidungsneuronen als dominante Ursache der untersuchten transienten Abrufstörungen.

B.5.4 untersuchte anschließend die bereits vorhandenen lokalen Recoverymechanismen.

### Ergebnisse aus B.5.4

In der unabhängigen Bestätigung:

| Variante | E1 X | E1 Y | Gemeinsame Verfügbarkeit |
|---|---:|---:|---:|
| R0 – historische Referenz | 43,09 % | 44,11 % | 18,05 % |
| R1 – unabhängige Recovery | 73,96 % | 74,60 % | 35,81 % |
| R2 – Recovery mit 50 % Relaxation | 76,42 % | 73,95 % | 53,93 % |

R2 verbessert die gemeinsame Verfügbarkeit gegenüber R1 um 18,12 Prozentpunkte.

Die zusätzliche Verbesserung des durchschnittlichen E1-Erfolgs gegenüber R1 ist dagegen statistisch nicht eindeutig.

Keine Variante erfüllt bislang das übergeordnete Robustheitsziel von 48/64 gleichzeitig erfolgreichen Seeds je Stromfamilie und Zuordnung.

### Identifizierter Architekturengpass

Die vorhandenen Recoverymechanismen werden nur nach einem neuronalen Feuerereignis geplant.

Ein Neuron, dessen Eingänge seine Membran verändern, ohne die Feuergrenze zu erreichen, plant dagegen keine Recovery.

Daraus folgt:

**Unterschwellige Membranpotenziale können ohne weitere geeignete Ereignisse zeitlich unbegrenzt bestehen bleiben.**

Diese Eigenschaft ist technisch nachgewiesen.

Noch nicht nachgewiesen ist, welcher Anteil der verbliebenen Lern- und Abrufprobleme tatsächlich durch diese unterschwelligen Restpotenziale verursacht wird.

Die Einführung einer neuen Relaxationsfunktion ist daher noch nicht gerechtfertigt.

---

## 2. Zentrale Forschungsfrage

**Kann eine ausschließlich lokal und ereignisgesteuert arbeitende Relaxation unterschwelliger Membranpotenziale die autonome Verfügbarkeit erlernter neuronaler Beziehungen verbessern, ohne notwendige zeitliche Integration oder die Fähigkeit zum weiteren Lernen zu beeinträchtigen?**

Dazu sind drei Teilfragen zu trennen:

1. Welche unterschwelligen Membranzustände verursachen tatsächlich unerwünschte spätere Entscheidungen?
2. Welche unterschwelligen Zustände sind dagegen für korrekte zeitliche Integration erforderlich?
3. Kann eine lokale Regel zwischen kurzfristig nützlicher Aktivität und längerfristig störender Restaktivität funktional vermitteln, ohne zu wissen, welche Handlung richtig ist?

Die dritte Frage darf nicht durch externe Zielinformationen beantwortet werden.

---

## 3. Forschungshypothesen

### H1 – Persistente unterschwellige Restpotenziale

Ein Teil der verbleibenden Abrufstörungen entsteht, weil frühere unterschwellige Aktivität den Schwellenvergleich bei einer späteren Reizpräsentation verändert.

### H2 – Nützliche zeitliche Integration

Dasselbe Restpotenzial kann erforderlich sein, damit zwei zeitlich getrennte schwache Eingänge gemeinsam ein Feuerereignis auslösen.

### H3 – Lokale zeitliche Begrenzung

Eine lokale, zeitlich verzögerte Relaxation könnte unerwünschte Restpotenziale begrenzen, ohne kurzfristige Integration vollständig zu verhindern.

### H4 – Abhängigkeit vom Relaxationszeitpunkt

Der Zeitpunkt einer lokalen Relaxation könnte wichtiger sein als ihre nominelle prozentuale Stärke.

### H5 – Zusammenspiel mit bestehender Recovery

Eine zusätzliche subthreshold-Relaxation kann mit R1 und R2 wechselwirken.

Die vorhandene feuergebundene Recovery und eine mögliche unterschwellige Relaxation müssen unabhängig voneinander bewertet werden.

### H6 – Indirekte Veränderung der Plastizität

Wird der Membranzustand verändert, können sich nachfolgende Feuerereignisse, Eligibility-Spuren und synaptische Lernänderungen ebenfalls verändern.

### H7 – Verschobene neuronale Konkurrenz

Eine Relaxation kann einen Entscheidungsweg verbessern und gleichzeitig einen anderen schwächen.

Eine höhere Einzelreizquote wäre dann keine ausreichende Verbesserung des gesamten Netzes.

### H8 – Ereignis- und Ressourcenaufwand

Eine zusätzliche lokale Ereignisplanung kann bei hoher Eingangsaktivität einen unverhältnismäßigen Aufwand verursachen.

Eine funktionale Verbesserung muss daher auch hinsichtlich Ereignisanzahl, Warteschlange und Speicherbedarf bewertet werden.

---

## 4. Phase A – Ausgangsbasis und Regression

### A1 – Branch und Integrationsstand

Prüfe vor Arbeitsbeginn, dass B.5.4 vollständig nach `main` integriert wurde.

Erstelle erst danach den Forschungsbranch:

`codex/research-stage-b55-subthreshold-membrane-relaxation`

Falls B.5.4 noch nicht integriert ist, dokumentiere den Blocker. Beginne keine Forschung auf einer fälschlich als konsolidiert bezeichneten Grundlage.

### A2 – Referenzen

Reproduziere:

- B.5.3 und B.5.4.
- R0 – historische Referenz.
- R1 – unabhängige lokale Recovery.
- R2 mit 0 % Relaxation.
- R2 mit 50 % Relaxation.
- No-Learning.
- Normale und inverse Zielzuordnung.
- Drei unabhängige Zufallsstromfamilien.

Die vollständige bisherige Testsuite muss erhalten bleiben.

### A3 – Unveränderte Architektur

Die Referenz behält:

- 32 Neuronen.
- Feuergrenze 2.
- Gewichtsgrenze 3.
- `ResetMode::to_zero`.
- Lokales G3.
- `active_phase`-Eligibility.
- Drei-Faktor-Plastizität.
- Geschützte Exploration.
- Lernbare inhibitorische Synapsen.
- Deaktiviertes Synapsenwachstum.

Für den zentralen Vergleich dient die bereits wirksame R2-50-Konfiguration aus B.5.4 als experimentelle Referenz.

Sie ist ausdrücklich kein freigegebener Produktionsstandard.

---

## 5. Phase B – Tatsächliche subthreshold-Zustände bestimmen

**Diese Phase muss abgeschlossen werden, bevor eine neue lokale Relaxationsregel implementiert wird.**

### B1 – Zustandsinventur

Erfasse unter natürlichen Lernbedingungen:

- Neuron.
- Neuronenrolle.
- Membranpotenzial vor einem Eingang.
- Membranpotenzial nach dem Eingang.
- Feuergrenze.
- Feuerentscheidung.
- Letztes lokales Ereignis.
- Vorhandene Recoveryplanung.
- Zeitpunkt des nächsten Eingangs.
- Zustand beim nächsten Eingang.

Dabei sind neuronale Entscheidungen und technische Diagnoseinformationen zu trennen.

### B2 – Unterschwellige Restpotenziale

Identifiziere Zustände mit:

`0 < membrane_potential < firing_threshold`

Diese Zustände sind nicht automatisch fehlerhaft.

Untersuche ihre Häufigkeit und die Situationen, in denen sie bestehen bleiben.

### B3 – Diskrete Werte

Bei einer Feuergrenze von 2 kann ein nichtnegatives unterschwelliges Membranpotenzial im bisherigen ganzzahligen Modell nur den Wert 1 besitzen.

Dieser Umstand ist ausdrücklich bei der Versuchsplanung zu berücksichtigen.

Insbesondere können mehrere unterschiedliche prozentuale Relaxationswerte auf denselben ganzzahligen Endwert abbilden.

Eine nominelle Parametervielfalt darf nicht mit tatsächlich unterschiedlichen neuronalen Wirkungen verwechselt werden.

### B4 – Positive und negative Folgeeffekte

Klassifiziere nachträglich:

- Restpotenzial erleichtert später eine korrekte Antwort.
- Restpotenzial begünstigt später eine falsche Antwort.
- Restpotenzial erzeugt eine Doppelantwort.
- Restpotenzial hat keinen beobachtbaren Effekt.
- Restpotenzial ist für die Integration mehrerer schwacher Eingänge erforderlich.

Die Klassifikation erfolgt ausschließlich in der externen Forschungsauswertung.

Sie darf dem neuronalen System nicht als Lerninformation zur Verfügung stehen.

---

## 6. Phase C – Kausale Prüfung unterschwelliger Restaktivität

### C1 – Reale Zustandskopien

Identifiziere natürliche Situationen, in denen ein Entscheidungsneuron ein unterschwelliges Restpotenzial besitzt.

Erzeuge vollständige Kopien vor dem nächsten tatsächlichen Reiz.

### C2 – Diagnostische Gegenproben

Vergleiche:

**D0 – Natürlich:** Unveränderter neuronaler Zustand.

**D1 – Lokale Entfernung:** Ausschließlich das konkrete unterschwellige Membranpotenzial wird diagnostisch auf null gesetzt.

**D2 – Umgekehrte Intervention:** Dasselbe Potenzial wird in einem passenden Kontrollzustand diagnostisch eingefügt.

**D3 – Ohne weitere Plastizität:** Derselbe Vorgang ohne synaptische Lernänderung.

**D4 – Beide Beziehungen:** Der andere Reiz wird in einer separaten identischen Kopie geprüft.

Alle übrigen Zustände sind kontrolliert gleich zu halten.

### C3 – Notwendigkeit und Hinreichendheit

Prüfe:

- Verhindert die Entfernung des Restpotenzials eine Fehlentscheidung?
- Verhindert sie eine korrekte Entscheidung?
- Kann das isolierte Einfügen des Restpotenzials einen zuvor korrekten Abruf verändern?
- Ist die Wirkung von konkurrierenden sensorischen oder inhibitorischen Gewichten abhängig?

### C4 – Keine Erfolgsselektion

Die Versuchsgruppe wird vorab definiert.

Es dürfen nicht ausschließlich solche Restpotenziale untersucht werden, deren Entfernung bereits eine Verbesserung zeigt.

### C5 – Entscheidungspunkt

Wenn kein relevanter kausaler Nachteil unterschwelliger Restpotenziale nachgewiesen wird, ist keine neue Produktionsfunktion zu implementieren.

B.5.5 darf dann mit einem negativen Forschungsbefund abgeschlossen werden.

---

## 7. Phase D – Zeitliche Integration als Gegenkontrolle

Jede mögliche Relaxationsregel muss die zeitliche Integration berücksichtigen.

### D1 – Einzelne schwache Eingänge

Untersuche Eingangssignale, die einzeln keine Feuerentscheidung ermöglichen.

### D2 – Wiederholte Eingänge

Untersuche:

- A → A.
- B → B.
- A → B.
- B → A.
- A → B → A.
- B → A → B.

Die Eingangskanäle sind technische Stimuli, keine fest programmierten Zielhandlungen.

### D3 – Zeitabstände

Mindestens:

- 0
- 1
- 2
- 4
- 8
- 16
- 32
- 64

simulierte Zeiteinheiten.

### D4 – Integrationsgrenze

Bestimme, wie lange ein unterschwelliges Potenzial erhalten bleiben muss, damit mehrere Eingänge zusammen eine korrekte neuronale Integration ermöglichen.

Hier wird keine feste biologische Gedächtnisdauer angenommen.

### D5 – Gegenläufige Effekte

Dokumentiere ausdrücklich Fälle, in denen eine schnellere Relaxation:

- Störende Restaktivität beseitigt.
- Notwendige Integration zerstört.
- Beide Effekte gleichzeitig auslöst.

Ein Mechanismus, der lediglich alle unterschwelligen Zustände möglichst schnell entfernt, ist nicht ausreichend begründet.

---

## 8. Phase E – Bedingte lokale Forschungsvarianten

**Erst nach erfolgreichem Kausalnachweis aus den Phasen B bis D ist eine minimale neue Funktion als abschaltbare Forschungsvariante zulässig.**

Die zentrale Idee ist eine zeitliche Begrenzung lokaler unterschwelliger Aktivität.

Die konkrete Regel ist aus den Kausalbefunden abzuleiten.

### E1 – Ereignisgesteuerte Variante

Möglicher Forschungsansatz:

1. Ein Neuron empfängt einen Eingang.
2. Sein Membranpotenzial bleibt oberhalb null, aber unterhalb der Feuergrenze.
3. Ein lokales Relaxationsereignis wird für einen späteren Zeitpunkt geplant.
4. Weitere lokale Eingänge können die vorgesehene Wirkung verändern oder das Ereignis ungültig machen.
5. Ein gültiges Relaxationsereignis verändert ausschließlich den lokalen Membranzustand.

Eine solche Regel darf keine richtige Handlung kennen.

### E2 – Lokaler Lazy-Ansatz

Alternativ darf untersucht werden, ob dieselbe beabsichtigte zeitabhängige Membranwirkung erst beim nächsten tatsächlichen Eingang berechnet werden kann.

Dabei könnte das Neuron seine eigene letzte relevante Aktivitätszeit berücksichtigen.

Eine globale Aktualisierung aller Neuronen ist unzulässig.

Der Ansatz muss gegen eine tatsächlich ereignisbasierte Referenz auf semantische Gleichwertigkeit und unterschiedliche Ressourcenwirkungen geprüft werden.

### E3 – Keine vorzeitige Auswahl

Die Wahl zwischen geplantem Recoveryereignis und lokaler Berechnung beim nächsten Eingang darf nicht allein von Implementierungskomfort oder Laufzeitgeschwindigkeit abhängen.

Zuerst muss klar sein, welche neuronale Wirkung experimentell gewünscht und kausal begründet ist.

---

## 9. Phase F – Anforderungen an einen neuen lokalen Mechanismus

Falls eine Forschungsvariante implementiert wird, gelten folgende Bedingungen.

### F1 – Rein lokal

Jedes Neuron verwaltet ausschließlich seinen eigenen zustandsrelevanten Ablauf.

Keine globale Membranüberwachung.

Keine globale Liste zu normalisierender Neuronen.

### F2 – Keine globale Taktung

Es dürfen keine periodischen Updates sämtlicher Neuronen eingeführt werden.

### F3 – Ereignisbegrenzung

Eine Folge vieler Eingänge darf nicht unbegrenzt neue gültige Relaxationsereignisse anhäufen.

Prüfe lokale Ereignisgenerationen und deterministische Ungültigkeit veralteter Ereignisse.

### F4 – Umgang mit weiteren Eingängen

Die Forschungsregel muss explizit festlegen, was geschieht, wenn vor der Relaxation:

- Ein weiterer erregender Eingang eintrifft.
- Ein hemmender Eingang eintrifft.
- Das Neuron feuert.
- Ein neues unterschwelliges Potenzial entsteht.
- Das Potenzial bereits null erreicht hat.

### F5 – Zusammenspiel mit R1/R2

Eine zusätzliche subthreshold-Relaxation darf nicht unkontrolliert dieselbe Membran mehrfach relaxieren.

Die bestehende feuergebundene Recovery und die neue unterschwellige Behandlung müssen getrennt identifizierbar bleiben.

### F6 – Ereignisreihenfolge

Bei identischen Zeitstempeln muss festgelegt und getestet werden:

- Relaxation vor einem neuen Eingang.
- Relaxation nach einem neuen Eingang.
- Bestehende Recovery.
- Eligibility-Ablauf.
- Neuronales Feuerereignis.

Die Reihenfolge muss deterministisch sein.

### F7 – Gültigkeit

Keine negativen Membranpotenziale, keine ungültigen Ereignisgenerationen und keine unkontrollierten Warteschlangen.

### F8 – Persistenz

Alle neu eingeführten lokalen Zustände müssen vollständig gespeichert und deterministisch wiederhergestellt werden können.

---

## 10. Phase G – Kontrollierte Variantenmatrix

Die endgültige Matrix wird erst nach dem Kausalbefund festgelegt.

Mindestens zu berücksichtigen:

| Variante | Beschreibung |
|---|---|
| S0 | Historische R0-Referenz |
| S1 | B.5.4-R1 ohne Membranrelaxation |
| S2 | B.5.4-R2 mit 50 % Relaxation |
| S3 | Diagnostische Entfernung unterschwelliger Restpotenziale |
| S4 | Bedingte lokale subthreshold-Relaxation |
| S5 | Identischer Aufbau mit deaktivierter neuer Funktion |
| S6 | Gegebenenfalls alternative ereignisgesteuerte Implementierung |

S3 ist ausschließlich diagnostisch und keine zulässige Lernregel.

S4 bis S6 sind nur bei nachgewiesenem Bedarf umzusetzen.

### G1 – Zeitparameter

Für eine begründete subthreshold-Relaxation dürfen vorab festgelegte Verzögerungen verglichen werden:

- 1
- 4
- 8
- 16
- 32
- 64

simulierte Zeiteinheiten.

### G2 – Relaxationswirkung

Untersuche zunächst, welche verschiedenen ganzzahligen Membranwerte die Regel tatsächlich erzeugt.

Bei unterschwelligem Potenzial 1 können viele prozentuale Relaxationswerte identisch wirken.

Nur tatsächlich unterschiedliche lokale Zustandswirkungen rechtfertigen getrennte große Naturversuche.

### G3 – Keine Bestwertsuche

Eine nachträgliche Auswahl ausschließlich anhand der höchsten E1-Quote ist unzulässig.

Die Kandidaten müssen anhand von Kausalität, Integration, Robustheit und Ressourcenwirkung begründet werden.

---

## 11. Phase H – Natürliche Lernverläufe

Untersuche die verbleibenden geeigneten Forschungsvarianten unter natürlichen gemischten Erfahrungen.

### H1 – Aufgaben

- X→A.
- Y→B.
- Sequenziell X→A, danach Y→B.
- Sequenziell Y→B, danach X→A.
- Alternierende Reize.
- Zufällig gemischte Reize.
- Inverse Zielzuordnung.
- Umweltumkehr.
- Retention.

### H2 – Vergleichbarkeit

Verwende:

- Identische Ausgangstopologie.
- Gepaarte Seeds.
- Unabhängige Reiz- und Explorationsströme.
- Identische äußere Bewertungsbedingungen.
- Gleiche Trainingsdauer.

### H3 – Lernverlauf

Erfasse:

- Erste positive Erfahrung.
- Synaptische Gewichtsentwicklung.
- Unterschwellige neuronale Aktivität.
- Tatsächlich ausgeführte lokale Relaxation.
- Neuronales Feuern.
- Eligibility-Bildung.
- Erste autonome Fähigkeit.
- Fähigkeitsverluste.
- Wiedererwerb.
- Gemeinsame Abrufbarkeit.

### H4 – Trainingsdauer

Mindestens 512 natürliche Episoden pro zentralem Versuchslauf.

---

## 12. Phase I – Zwei Beziehungen und Stabilität

Die wichtigste Bewertung betrifft nicht den Erfolg eines einzelnen Pfades.

### I1 – Gemeinsamer Erwerb

Bestimme, wann X und Y erstmals gleichzeitig autonom korrekt sind.

### I2 – Gemeinsamer Erhalt

Prüfe den Zustand nach:

- 1 Erfahrung.
- 4 Erfahrungen.
- 8 Erfahrungen.
- 16 Erfahrungen.
- 32 Erfahrungen.
- 64 Erfahrungen.

### I3 – Verlustklassen

Unterscheide:

- Synaptisch verursachten Verlust.
- Transient verursachten Verlust.
- Inhibitorisch verursachten Verlust.
- Fehler durch konkurrierende Aktivität.
- Kombinierte Ursache.

### I4 – Selektive Verbesserung

Wenn ein Eingriff X verbessert, muss gleichzeitig Y geprüft werden.

Eine Verbesserung eines Pfades bei Verschlechterung des anderen darf nicht als allgemeine Gedächtnisstabilisierung gewertet werden.

### I5 – Natürliche Dynamik

Ein zeitweilig nicht korrekter Abruf darf nicht automatisch mit einer gelöschten Fähigkeit gleichgesetzt werden.

Die synaptische Speicherung und die aktuelle neuronale Nutzbarkeit sind weiterhin getrennt auszuwerten.

---

## 13. Phase J – Umweltumkehr und Lernfähigkeit

Eine neue lokale Relaxation darf die Anpassungsfähigkeit nicht beseitigen.

### J1 – Ursprüngliche Zuordnung

Untersuche zunächst den Erwerb einer Reiz-Handlungs-Beziehung.

### J2 – Umkehr

Ändere danach die äußere Bewertung.

Kein Reset.

Keine technische Zielinformation für das neuronale System.

### J3 – Umlernen

Prüfe:

- Bleibt Exploration möglich?
- Entsteht eine neue autonome Präferenz?
- Werden bestehende Verbindungen weiter verändert?
- Funktionieren Eligibility und Drei-Faktor-Plastizität?
- Beeinträchtigt zusätzliche Relaxation die Anpassung?

### J4 – Retention

Prüfe nach der Umkehr den autonomen Abruf ohne weitere Plastizität.

---

## 14. Phase K – Kausale Kontrollversuche

Jede relevante Verbesserung einer neuen Variante muss durch Zustandskopien eingegrenzt werden.

### K1 – Gleiche Gewichte, andere Relaxation

Vergleiche die Ergebnisse bei identischen Gewichten und identischem Ausgangszustand.

### K2 – Gleiche Eingangsfolge, andere Relaxationszeit

Prüfe den kausalen Einfluss des Zeitpunkts.

### K3 – Relaxation vor und nach einem Reiz

Untersuche:

- Relaxation bereits durchgeführt.
- Relaxation noch ausstehend.
- Relaxation ungültig geworden.
- Kein Relaxationsereignis vorhanden.

### K4 – Indirekte Plastizitätswirkung

Unterscheide:

- Unmittelbare Änderung der autonomen Antwort.
- Spätere Veränderung durch neue synaptische Lernereignisse.

### K5 – Gegenläufige Ergebnisse

Dokumentiere ausdrücklich auch Fälle, in denen eine neue Relaxation korrekte Antworten verhindert.

Diese Fälle dürfen nicht aus den Erfolgsstatistiken entfernt werden.

---

## 15. Statistische Versuchsgestaltung

### Entdeckung

Mindestens:

- 64 gepaarte Seeds.
- Drei unabhängige Zufallsstromfamilien.
- Normale und inverse Zielzuordnung.

### Bestätigung

Mindestens weitere 64 zuvor unbenutzte Seeds je zentraler Kombination.

### Priorität der Vergleiche

1. Unveränderte B.5.4-Referenz.
2. Kausal isolierte subthreshold-Intervention.
3. Gegebenenfalls neue lokale Forschungsvariante.
4. Abschaltkontrolle.
5. Unabhängige Bestätigung.

### Statistische Anforderungen

- Gepaarte Erfolgsunterschiede.
- 95-%-Bootstrap-Konfidenzintervalle.
- Clusterbewusste Auswertung.
- Getrennte Werte für X und Y.
- Gemeinsame Seed-Robustheit.
- Retentionsverläufe.
- Verwechslungsmatrizen.
- Zeiten bis zum ersten Erwerb.
- Zeiten bis zum Verlust.
- Anzahl sinnvoller und störender Relaxationsereignisse.

Mehrere Ereignisse desselben Netzes dürfen nicht als unabhängige Seeds gezählt werden.

---

## 16. Allgemeine Erfolgskriterien

Für eine neue neuronale Funktion bleiben die bisherigen Anforderungen bestehen:

- Mindestens 75 % E1 für X.
- Mindestens 75 % E1 für Y.
- Mindestens 48/64 Seeds erfüllen beide Schwellen gleichzeitig.
- Mindestens 15 Prozentpunkte gepaarter Lernvorteil gegenüber No-Learning.
- Vollständig positives 95-%-Konfidenzintervall.
- Bestätigung durch unabhängige Seeds.
- Normale und inverse Zuordnung.
- Drei unabhängige Erfahrungsstromfamilien.
- Retention ohne weitere Plastizität.
- Erhalt der Umweltanpassungsfähigkeit.

### Zusätzliches B.5.5-Kriterium

Eine neue subthreshold-Relaxation muss einen eigenen positiven kausalen Beitrag gegenüber der unveränderten B.5.4-R2-Referenz nachweisen.

Eine Verbesserung ausschließlich gegenüber der alten R0-Referenz reicht nicht aus.

### Integrationskriterium

Die zeitliche Integration schwacher Eingangssignale darf nicht grundsätzlich zerstört werden.

Ein funktionaler Unterschied muss ausdrücklich hinsichtlich nützlicher und störender Integration bewertet werden.

### Wissenschaftlicher Abschluss

B.5.5 darf ohne neue Funktion abgeschlossen werden.

Ein negativer Befund ist zulässig, sofern die zugrunde liegende Frage nachvollziehbar untersucht wurde.

---

## 17. Ressourcenanforderungen

Eine mögliche neue lokale Relaxation muss ressourceneffizient bleiben.

Erfasse:

- Anzahl geplanter Ereignisse.
- Anzahl tatsächlich ausgeführter Ereignisse.
- Anzahl ungültiger Ereignisse.
- Zusätzliche Ereignisse pro Eingang.
- Zusätzliche Ereignisse pro Trainingsepisode.
- Maximale Queuegröße.
- Speicherbedarf pro Neuron.
- Gesamtspeicherbedarf.
- CPU-Zeit.
- Einfluss auf deterministische Verarbeitung.

### Ereignisbegrenzung

Unkontrolliertes Wachstum der Queue ist unzulässig.

Eine hohe Eingangsfrequenz darf nicht zu einer unbegrenzten Ansammlung gültiger Relaxationsereignisse führen.

### Hardwareorientierung

Die langfristige Eignung für ressourcenbeschränkte Systeme bleibt zu berücksichtigen.

Eine lokale Relaxationsregel, die umfangreiche globale Zustandsverwaltung verlangt, widerspricht dem Forschungsziel.

---

## 18. Persistenz und Regression

Alle bisherigen Kerntests müssen bestehen bleiben.

Für B.5.5 sind zusätzliche Tests erforderlich für:

- Subthreshold-Potenzial ohne Feuer.
- Subthreshold-Potenzial mit späterem Eingang.
- Diagnostische Entfernung eines Restpotenzials.
- Diagnostische Wiederherstellung eines Restpotenzials.
- Positive zeitliche Integration.
- Störende zeitliche Integration.
- Lokale Ereignisplanung.
- Veraltete Relaxationsgeneration.
- Mehrere Eingänge vor Relaxation.
- Feuern vor Relaxation.
- Wechselwirkung mit vorhandener Recovery.
- Gleichzeitige Ereigniszeitstempel.
- Inhibitorische Verarbeitung.
- Normale und inverse Zuordnung.
- Geschützte Exploration.
- Retention.
- Umweltumkehr.
- Identische Zustandskopien.
- Persistenz ausstehender Relaxationsereignisse.
- Deterministische Fortsetzung.
- Gültige Netzwerkzustände.
- Begrenzte Warteschlange.

Falls keine neue lokale Funktion implementiert wird, sind nicht anwendbare Funktionstests ausdrücklich als solche zu kennzeichnen.

Historische Forschungsdaten dürfen nicht überschrieben werden.

---

## 19. Architekturgrenzen

Unverändert gelten:

- C++17.
- Vollständig ereignisgesteuerte Verarbeitung.
- Kein globaler neuronaler Tick.
- Binäre Feuerereignisse.
- Gewichtete erregende und hemmende Synapsen.
- Lokales G3.
- Zeitlich begrenzte Eligibility.
- Drei-Faktor-Plastizität.
- Geschützte Exploration.
- Keine vorgegebene Reiz-Handlungs-Zuordnung.
- Keine globale Motorselektion.
- Keine technischen Gewinnerentscheidungen.
- Kein globaler Membranreset.
- Keine Wiederherstellung früher erfolgreicher Gesamtzustände.
- Keine pauschale Sicherung gelernter Gewichte.
- Keine permanenten Zeitstempel pro Synapse.
- Keine neue globale neuronale Zustandsverwaltung.
- Vollständige Persistenz.

Eine neue lokale Funktion darf nur aus den tatsächlich verfügbaren neuronalen Zuständen heraus wirken.

---

## 20. Lieferumfang

Nova-EVE erstellt:

1. Technische Analyse vorhandener subthreshold-Zustände.
2. Vollständige Reproduktion der B.5.4-Referenz.
3. Statistische Erfassung natürlicher unterschwelliger Restpotenziale.
4. Kausale Zustandskopien für positive und negative Folgeeffekte.
5. Untersuchung zeitlicher Integration.
6. Bedarfsentscheidung für eine neue lokale Funktion.
7. Gegebenenfalls minimal implementierte experimentelle Relaxation.
8. Abschaltkontrolle und Kontrollvarianten.
9. Ereignis-, Generationen- und Zeitabstandstests.
10. Natürliche gemischte Lernverläufe.
11. Untersuchung beider Reizbeziehungen.
12. Retentions- und Umweltumkehrtests.
13. Unabhängige Bestätigungsversuche.
14. Ressourcen- und Persistenzmessungen.
15. Vollständige Rohdaten und statistische Auswertungen.
16. Regressionstests.
17. Forschungsbericht B.5.5.
18. Technische Analyse der lokalen Membrandynamik.
19. Entscheidungsvorlage für B.5.6.

---

## 21. Abschlussfragen

**A.** Wie häufig entstehen unter natürlichen Erfahrungen unterschwellige Restpotenziale?

**B.** Wie häufig haben diese Restpotenziale einen nachweislich positiven beziehungsweise negativen Einfluss auf den nächsten autonomen Abruf?

**C.** Welcher Anteil der verbleibenden Abrufstörungen ist kausal durch unterschwellige Restaktivität erklärbar?

**D.** Welche Zeitabstände sind für sinnvolle Integration erforderlich?

**E.** Kann eine lokale zeitliche Begrenzung störende Aktivität reduzieren, ohne notwendige Integration zu zerstören?

**F.** Reicht eine ereignisgesteuerte Lösung aus oder ist eine lokale Berechnung beim nächsten Eingang geeigneter?

**G.** Welche Rolle spielen inhibitorische Eingänge?

**H.** Welche Wechselwirkungen entstehen mit R1/R2?

**I.** Wird synaptisches Lernen durch veränderte Feuerhäufigkeiten beeinflusst?

**J.** Können beide Beziehungen gleichzeitig stabiler verfügbar werden?

**K.** Bleibt Umlernen nach einer Umweltumkehr möglich?

**L.** Ist eine neue lokale Funktion notwendig und durch die Kausalbefunde gerechtfertigt?

**M.** Kann diese Funktion mit begrenztem Speicher- und Ereignisaufwand umgesetzt werden?

**N.** Welche konkrete offene Frage ergibt sich für B.5.6?

---

## 22. Abschlussentscheidung und Merge-Empfehlung

Der Forschungsbericht muss ausdrücklich enthalten:

**Empfehlung zum Merge von B.5.5 nach `main`: Ja oder Nein.**

Diese Empfehlung ist unabhängig davon zu treffen, ob eine neue neuronale Funktion freigegeben wird.

### Merge empfohlen, wenn:

- Der Forschungsstand technisch abgeschlossen ist.
- Historische Regressionen bestehen.
- Diagnose und Produktionsmechanismen sauber getrennt sind.
- Sämtliche Änderungen nachvollziehbar dokumentiert sind.
- Persistenz und deterministische Fortsetzung erhalten bleiben.
- Keine ungelösten technischen Blocker bestehen.

### Merge nicht empfohlen, wenn:

- Die Daten nicht reproduzierbar sind.
- Der neuronale Kern unbeabsichtigt verändert wurde.
- Experimentelle Mechanismen ohne Freigabe aktiv werden.
- Persistenz oder Determinismus beschädigt sind.
- Kritische Regressionen fehlschlagen.

Ein negativer wissenschaftlicher Befund verhindert den Merge nicht automatisch.

Eine unbewiesene Forschungsvariante darf beim Merge jedoch nicht stillschweigend zum Produktionsstandard werden.

---

## 23. Forschungsleitlinie

**Nicht jede verbleibende neuronale Aktivität ist störend. Und nicht jede fehlende Antwort bedeutet verlorenes Wissen.**

Unterschwellige Membranpotenziale ermöglichen es einem Neuron, mehrere zeitlich getrennte Eingänge gemeinsam zu berücksichtigen.

Dieselbe Eigenschaft kann spätere Entscheidungen aber auch beeinflussen, obwohl die ursprüngliche Aktivität für die aktuelle Situation möglicherweise nicht mehr relevant ist.

Für EVE suchen wir keine technisch erzwungene richtige Membranspannung.

Wir untersuchen eine lokale Dynamik, die:

- Neue Erfahrungen aufnimmt.
- Frühere Aktivität sinnvoll berücksichtigt.
- Zeitliche Zusammenhänge verarbeiten kann.
- Unbeabsichtigte dauerhafte Erregungszustände begrenzt.
- Konkurrierende Fähigkeiten verfügbar hält.
- Weiteres Lernen und Umlernen ermöglicht.
- Ohne globale Steuerung und ohne vorgegebene Bedeutungen auskommt.

**Das Ziel von B.5.5 ist nicht, die Membran möglichst schnell zu entladen. Das Ziel ist, herauszufinden, ob eine lokal begrenzte zeitliche Wirkung neuronaler Aktivität für zuverlässiges Lernen und Erinnern erforderlich ist – und welche Regeln sich dafür tatsächlich aus den Experimenten ableiten lassen.**
