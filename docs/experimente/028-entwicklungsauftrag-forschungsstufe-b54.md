# Entwicklungsauftrag 028 – Forschungsstufe B.5.4

## Lokale Membran-Recovery und zeitliche Stabilisierung neuronaler Entscheidungen

**Projekt:** EVE – Emergent Virtual Evolution  
**Technologie:** C++17  
**Ausgangsbasis:** Konsolidierter Forschungsstand B.5.3 auf `main`  
**Forschungsbranch:** `codex/research-stage-b54-local-membrane-recovery`  
**Status:** Kontrollierte Untersuchung vorhandener neuronaler Mechanismen  
**Freigabe:** Keine neue allgemeine Lern- oder Gedächtnisfunktion vorab genehmigt

---

## 1. Ausgangslage

Die Forschungsstufen B.5.1 bis B.5.3 haben den Erwerb, Verlust und Wiedererwerb neuronaler Reiz-Handlungs-Beziehungen untersucht.

B.5.3 konnte die dominierende Ursache der zuvor beobachteten transienten Abrufstörungen auf die lokale Membranlage der Entscheidungsneuronen eingrenzen.

### Gesicherte experimentelle Befunde

In 15.194 detaillierten Verlustzuständen:

- 14.731 Antworten wurden durch Wiederherstellung der früheren Membranpotenziale wieder korrekt.
- 14.636 zuvor korrekte Antworten wurden durch Übernahme der neuen Membranpotenziale gestört.
- 12.452 Verluste traten ohne synaptische Gewichtsänderung auf.
- 16.326 von 16.427 beobachteten Membranunterschieden betrafen Entscheidungsneuronen.
- In 14.728 wiederherstellbaren Fällen genügte die Rücknahme eines einzelnen Entscheidungsmembranwertes.
- Nur drei wiederherstellbare Fälle benötigten gemeinsam zwei Membranänderungen.

Andere untersuchte transiente Zustandskomponenten hatten in der Referenzkonfiguration keine entsprechende unmittelbare E1-Wirkung.

Insbesondere G3, Eligibility und synaptische Aktivitätszähler beeinflussten den unmittelbaren Abruf nicht direkt.

Diese Komponenten bleiben dennoch für spätere neuronale Lernverarbeitung relevant.

### Bedeutung der Ergebnisse

Die vorhandenen synaptischen Verbindungen können erhalten bleiben, während der aktuelle Membranzustand eine zuvor mögliche motorische Antwort verändert oder verhindert.

Das bedeutet nicht, dass Membranpotenziale grundsätzlich nachteilig sind.

Sie ermöglichen auch zeitliche Integration und können den Abruf nach weiteren Eingängen verbessern.

Eine geeignete Membrandynamik muss beide Eigenschaften berücksichtigen:

1. Vorherige neuronale Aktivität sinnvoll integrieren.
2. Unbeabsichtigte dauerhafte Verzerrungen späterer Entscheidungen begrenzen.

---

## 2. Wichtiger technischer Vorbefund

Vor Beginn der eigentlichen Recovery-Vergleiche muss eine mögliche Inkonsistenz zwischen Forschungsdokumentation und tatsächlicher Implementierung geklärt werden.

### Vorhandene Recovery-Modi

Die Architektur definiert:

- `RecoveryMode::input_dependent`
- `RecoveryMode::independent`
- `RecoveryMode::independent_with_membrane_relaxation`

Dazu existieren:

- `refractory_input_events`
- `recovery_delay`
- `membrane_relaxation_percent`
- `reset_mode`

### Problem der bisherigen Referenz

Der aktuelle Forschungsaufbau setzt:

`refractory_input_events = 0`

Die vorhandene Funktion `Brainstem::recover_neuron()` behandelt ein Recoveryereignis unter anderem dann als veraltet, wenn:

`refractory_events_remaining == 0`

Damit wird die Membranrelaxation bei null Refraktärereignissen nicht wirksam, selbst wenn ein zeitgesteuertes Recoveryereignis eingeplant wurde.

**Die bloße Aktivierung eines anderen Recovery-Modus reicht in der bisherigen Referenzkonfiguration daher nicht aus, um eine wirksame Membranrelaxation zu untersuchen.**

### Zweite mögliche Inkonsistenz

Die B.5.3-Dokumentation beschreibt ein Membran-Resetverfahren `subtract_threshold`.

Im aktuellen Konfigurationspfad wird jedoch nicht ausdrücklich `ResetMode::subtract_threshold` gesetzt; der Standardwert der Konfigurationsstruktur ist `ResetMode::to_zero`.

Vor allen Versuchen muss deshalb die tatsächlich verwendete Resetmethode verifiziert werden.

Es darf keine stillschweigende Korrektur der Referenz stattfinden.

---

## 3. Zentrale Forschungsfrage

**Kann eine bereits vorhandene lokale, ereignisgesteuerte Recovery-Funktion die langfristige Verfügbarkeit erlernter neuronaler Beziehungen verbessern, ohne die notwendige zeitliche Integration, das Lernen weiterer Beziehungen oder die Anpassungsfähigkeit des Netzwerks zu beeinträchtigen?**

Dabei sind vier Effekte getrennt zu untersuchen:

1. Lokale Refraktärwirkung.
2. Ereignisgebundene Recovery.
3. Zusätzliche Membranrelaxation.
4. Wechselwirkungen mit synaptischer Plastizität und konkurrierender Aktivität.

Ein besserer einzelner Abruf genügt nicht als Erfolgsnachweis.

---

## 4. Forschungshypothesen

### H1 – Dauerhafte Restaktivität

Ohne wirksame lokale Recovery können Membranpotenziale zwischen sensorischen Ereignissen erhalten bleiben und nachfolgende Entscheidungen beeinflussen.

### H2 – Recovery ohne Relaxation

Eine lokale Recovery, die lediglich einen Refraktärzustand beendet, verändert das neuronale Verhalten anders als eine Recovery mit zusätzlicher Membranrelaxation.

### H3 – Membranrelaxation

Eine begrenzte lokale Membranrelaxation kann unerwünschte Restaktivität reduzieren und die Abrufstabilität erhöhen.

### H4 – Verlust zeitlicher Integration

Eine zu starke oder zu schnelle Relaxation kann notwendige zeitliche Integration beeinträchtigen und damit erworbene Fähigkeiten verschlechtern.

### H5 – Lokale Konkurrenz

Eine veränderte Membrandynamik kann beide Entscheidungswege unterschiedlich beeinflussen und dadurch auch bei unveränderten synaptischen Gewichten neue Konkurrenzsituationen erzeugen.

### H6 – Wechselwirkung mit Lernen

Eine veränderte Feuerrate und Ereignisreihenfolge kann die Eligibility-Bildung und damit indirekt die spätere synaptische Plastizität verändern.

### H7 – Refraktärzustand als eigenständiger Faktor

Eine etwaige Verbesserung kann durch den zusätzlichen Refraktärzustand entstehen, ohne dass Membranrelaxation ursächlich ist.

### H8 – Grenzen der vorhandenen Recovery-Architektur

Die vorhandene Recovery wird ausschließlich nach bestimmten Feuerereignissen wirksam.

Sie könnte deshalb Restpotenziale von Neuronen, die keinen neuen Schwellenübergang erreichen, grundsätzlich nicht angemessen beeinflussen.

Alle Hypothesen sind getrennt und mit kontrollierten Zustandskopien zu prüfen.

---

## 5. Phase A – Technisches Recovery-Audit

**Diese Phase ist verpflichtend und muss vor der großen Versuchsmatrix abgeschlossen werden.**

### A1 – Konfigurationspfad

Verfolge die tatsächlich verwendeten Werte vom Forschungsrunner bis in die neuronale Verarbeitung:

- `reset_mode`
- `refractory_input_events`
- `recovery_mode`
- `recovery_delay`
- `membrane_relaxation_percent`

Dokumentiere alle Standardwerte und tatsächlich explizit gesetzten Werte.

### A2 – Recovery-Ereigniskette

Untersuche:

1. Wann wird ein Recoveryereignis geplant?
2. Welche Neuronen planen Recovery?
3. Welcher Refraktärzustand entsteht dabei?
4. Unter welchen Bedingungen wird Recovery durchgeführt?
5. Wann gilt ein Ereignis als veraltet?
6. Wann wird das Membranpotenzial tatsächlich verändert?
7. Wie wirken unterschiedliche Recovery-Generationen?
8. Was passiert bei erneutem Feuern vor einer geplanten Recovery?
9. Wie verhalten sich Recovery und Eligibility-Ablauf bei gleichem Zeitstempel?

### A3 – Reset-Semantik

Verifiziere, ob die B.5.3-Referenz tatsächlich `to_zero` oder `subtract_threshold` verwendet.

Falls die Forschungsdokumentation unzutreffend ist:

- Dokumentiere die Abweichung ausdrücklich.
- Korrigiere die technische Beschreibung.
- Verändere die historische Referenz nicht.
- Prüfe, ob bisherige Schlussfolgerungen davon betroffen sind.

### A4 – Tatsächliche Recovery-Wirksamkeit

Erstelle kleine deterministische Testnetze.

Beobachte jeweils:

- Feuerereignis.
- Membranpotenzial danach.
- Refraktärzustand.
- Geplantes Recoveryereignis.
- Eintreten des Recoveryereignisses.
- Ergebnis der Recovery.
- Membranpotenzial nach Recovery.

Prüfe insbesondere `refractory_input_events = 0`, `1` und `2`.

Ein Modus gilt erst dann als experimentell aktiv, wenn eine tatsächlich ausgeführte Recovery mit nachweisbarer Wirkung beobachtet wurde.

### A5 – Entscheidungspunkt

Falls sich bestätigt, dass R1 und R2 bei null Refraktärereignissen keine Membranrelaxation ausführen, darf keine große wirkungslose Parametermatrix gestartet werden.

Die Untersuchung wird dann auf technisch wirksame, kontrollierte Konfigurationen begrenzt.

Eine erforderliche Aktivierung von Refraktärereignissen ist als eigenständiger experimenteller Faktor zu behandeln.

---

## 6. Phase B – Referenzgruppen

### R0 – Unveränderte Referenz

- `RecoveryMode::input_dependent`
- `refractory_input_events = 0`
- Keine zusätzliche Membranrelaxation.
- Unverändertes Resetverfahren.
- Unveränderte Lernregeln.

Dies ist die historische B.5.3-Referenz.

### R1 – Unabhängige lokale Recovery

- `RecoveryMode::independent`
- Ereignisgebundene Recovery nach neuronaler Aktivität.
- Keine zusätzliche Membranrelaxation.

### R2 – Lokale Recovery mit Membranrelaxation

- `RecoveryMode::independent_with_membrane_relaxation`
- Konfigurierbare Recovery-Verzögerung.
- Konfigurierbare lokale Membranrelaxation.

### Kontrollierte Refraktärvarianten

Für die technische Untersuchung sind mindestens die Werte 0, 1 und 2 vorzusehen.

Dabei gilt:

**R0 mit Refraktärwert 0 bleibt die historische Referenz.**

Zusätzliche Konfigurationen mit veränderten Refraktärwerten sind eigene Forschungsvarianten.

R1 und R2 müssen jeweils auch bei identischem Refraktärwert und identischer Recovery-Verzögerung verglichen werden.

Nur so kann die zusätzliche Wirkung der Membranrelaxation gegenüber der allgemeinen Recovery isoliert werden.

### Kontrollgruppe R2-0

Prüfe R2 mit `membrane_relaxation_percent = 0`.

Dies dient zur Kontrolle, ob das Verhalten mit R1 bei identischen übrigen Parametern übereinstimmt.

---

## 7. Phase C – Kontrollierte Parameteruntersuchung

Es darf keine unbegrenzte Optimierung der Trefferquote durchgeführt werden.

Die Versuchsparameter werden vorab festgelegt.

### C1 – Recovery-Verzögerung

Untersuche zunächst die technisch gültigen Verzögerungen:

- 1 Zeiteinheit.
- 4 Zeiteinheiten.
- 8 Zeiteinheiten.
- 16 Zeiteinheiten.
- 32 Zeiteinheiten.
- 64 Zeiteinheiten.

### C2 – Membranrelaxation

Untersuche:

- 0 %
- 10 %
- 25 %
- 50 %
- 75 %
- 100 %

0 % ist eine Kontrollbedingung.

100 % stellt innerhalb des Recoveryereignisses eine vollständige Relaxation des aktuellen Membranpotenzials dar, jedoch keinen globalen Netzwerkreset.

### C3 – Stufenweises Vorgehen

Zunächst sind technische und lokale Verhaltensversuche durchzuführen.

Erst nach nachgewiesener Wirksamkeit folgt eine begrenzte Hauptmatrix.

Alle Parameterkombinationen müssen dokumentiert sein.

Eine nachträgliche Auswahl allein anhand der besten E1-Quote ist unzulässig.

### C4 – Zeitliche Überschneidungen

Besonders untersuchen:

- Recovery vor dem nächsten Reiz.
- Recovery während einer Reizfolge.
- Recovery gleichzeitig mit einem neuen Reiz.
- Recovery gleichzeitig mit Eligibility-Ablauf.
- Recovery nach zwischenzeitlich erneutem Feuern.

Bei gleichen Zeitstempeln muss die tatsächliche Ereignisreihenfolge deterministisch und nachvollziehbar sein.

---

## 8. Phase D – Lokale Membrandynamik

Untersuche die unmittelbare Wirkung auf neuronale Zustände.

### D1 – Einzelnes Neuron

Verwende ein Testneuron mit bekannten erregenden und hemmenden Eingängen.

Erfasse:

- Eingangssignal.
- Membranpotenzial vor Integration.
- Membranpotenzial danach.
- Feuerentscheidung.
- Zustand nach Feuerereignis.
- Lokale Recovery.
- Zustand nach Recovery.
- Antwort auf den nächsten Eingang.

### D2 – Subthreshold-Aktivität

Besonders wichtig ist die Untersuchung von Eingängen, die die Feuergrenze nicht erreichen.

Prüfe:

- Bleibt das Membranpotenzial erhalten?
- Wird überhaupt ein Recoveryereignis geplant?
- Kann die vorhandene Recovery diesen Zustand verändern?
- Ist eine solche Veränderung für späteren Abruf notwendig?

Wenn ein nicht feuerndes Neuron keine Recovery erhält, muss dies als strukturelle Grenze der vorhandenen Architektur dokumentiert werden.

### D3 – Wiederholtes Feuern

Untersuche:

- Ein einzelnes Feuerereignis.
- Zwei kurz aufeinanderfolgende Feuerereignisse.
- Wiederholtes Feuern vor einer geplanten Recovery.
- Mehrere noch ausstehende Recoveryereignisse.
- Veraltete Recovery-Generationen.

Es dürfen keine unkontrollierten mehrfachen Relaxationen durch veraltete Recoveryereignisse entstehen.

### D4 – Inhibitorische Wirkung

Untersuche die lokale Integration bei:

- Rein erregendem Eingang.
- Rein hemmendem Eingang.
- Gleichzeitiger Erregung und Hemmung.
- Hemmung nach vorausgegangener Erregung.
- Erregung nach vorausgegangener Hemmung.

Die Membranrelaxation darf die grundlegende Unterscheidung zwischen erregenden und hemmenden Eingängen nicht aufheben.

---

## 9. Phase E – Zeitliche Integration erhalten

Dieser Abschnitt ist ein eigenständiges Erfolgskriterium.

### E1 – Zwei schwache Eingangssignale

Erzeuge eine kontrollierte Konfiguration, in der zwei einzelne Eingänge für sich genommen keine Feuerentscheidung auslösen, ihre zeitliche Integration jedoch ausreichend ist.

Prüfe die Erfolgswahrscheinlichkeit bei verschiedenen zeitlichen Abständen.

### E2 – Unterschiedliche Signalreihenfolgen

Untersuche:

- A → A.
- B → B.
- A → B.
- B → A.
- A → B → A.
- B → A → B.

Dabei sind die Buchstaben zunächst allgemeine Eingangskanäle, keine technisch hinterlegten richtigen Motorentscheidungen.

### E3 – Zeitliche Abstände

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

### E4 – Integrationsverlust

Erfasse, ob eine Recovery-Variante notwendige zeitliche Integration verhindert.

Eine Variante darf nicht allein deshalb als besser gelten, weil sie frühere Aktivität schneller beseitigt.

### E5 – Positive Voraktivität

Untersuche ausdrücklich Situationen, in denen vorherige Membranaktivität die nächste korrekte Entscheidung begünstigt.

Diese Fälle dienen als Gegenkontrolle gegen eine pauschale Annahme schädlicher Restpotenziale.

---

## 10. Phase F – Natürliche Lernverläufe

Untersuche die technisch wirksamen Recovery-Varianten in den bestehenden Lernaufgaben.

### F1 – Vergleichbare Anfangsbedingungen

Alle Varianten erhalten:

- Gleiche Ausgangstopologie.
- Gleiche initiale synaptische Gewichte.
- Gleiche Zielzuordnung.
- Gepaarte Seeds.
- Kontrollierte unabhängige Reiz- und Explorationsströme.

Abweichende neuronale Zustände nach unterschiedlichen Erfahrungen sind erwartete Versuchseffekte.

Sie dürfen nicht künstlich angeglichen werden.

### F2 – Lernverlauf

Erfasse pro Episode:

- Reiz.
- Autonome Handlung.
- Explorative Handlung.
- Äußere Bewertung.
- Angewandte Modulation.
- Sensorische Gewichtsentwicklung.
- Inhibitorische Gewichtsentwicklung.
- Membranpotenziale.
- Tatsächlich ausgeführte Recoveryereignisse.
- Veraltete Recoveryereignisse.
- Eligibility-Entstehung.
- Neuronale Feuerhäufigkeit.

### F3 – Erwerb

Untersuche:

- Zeitpunkt des ersten autonomen Erwerbs.
- Anzahl notwendiger positiver Erfahrungen.
- Stabilität nach Erwerb.
- Wiedererwerb nach Verlust.

### F4 – Interferenz

Untersuche, ob die geänderte Membrandynamik die andere Reiz-Handlungs-Beziehung verbessert oder beeinträchtigt.

### F5 – Langfristiger Verlauf

Mindestens 512 natürliche Trainingsepisoden.

Zusätzliche lange Versuche sind nur bei einer konkreten offenen Fragestellung erforderlich.

---

## 11. Phase G – Stabilität beider Beziehungen

Die allgemeine Zielsetzung ist nicht die Optimierung eines einzelnen Entscheidungswegs.

### G1 – Erstmaliger gemeinsamer Erfolg

Ermittle, wann beide Reiz-Handlungs-Beziehungen erstmals gleichzeitig autonom korrekt sind.

### G2 – Erhalt

Prüfe nach:

- 1 Erfahrung.
- 4 Erfahrungen.
- 8 Erfahrungen.
- 16 Erfahrungen.
- 32 Erfahrungen.
- 64 Erfahrungen.

### G3 – Verlustklassen

Unterscheide:

- Verlust bei veränderten Gewichten.
- Verlust bei identischen Gewichten.
- Verlust durch inhibitorische Konkurrenz.
- Verlust durch Membranzustände.
- Verlust durch mehrere Faktoren.

Die B.5.3-Diagnoseschnittstellen dürfen hierfür eingesetzt werden.

Sie dürfen nicht als automatische Korrekturfunktion in die Trainingsepisoden eingreifen.

### G4 – Gegenseitige Beeinträchtigung

Ein besserer Abruf von X bei gleichzeitig verschlechtertem Abruf von Y ist nicht als allgemeine Verbesserung zu bewerten.

Die gemeinsame Stabilität ist eigenständig zu messen.

---

## 12. Phase H – Lernfähigkeit und Umweltumkehr

Eine lokale Recovery darf Lernfähigkeit nicht nur dadurch verbessern, dass neuronale Aktivität insgesamt unterdrückt wird.

### H1 – Ursprüngliche Beziehung

Untersuche den autonomen Erwerb der ursprünglichen Zielzuordnung.

### H2 – Umweltumkehr

Ändere anschließend die äußere Bewertung der Handlungen.

Kein technischer Reset neuronaler Gewichte oder transienter Zustände.

### H3 – Umlernen

Prüfe:

- Entsteht eine neue autonome Präferenz?
- Bleibt geschützte Exploration erreichbar?
- Können zuvor aufgebaute Verbindungen verändert werden?
- Bleibt die lokale Eligibility wirksam?
- Wird das Umlernen durch häufige Recovery gestört?

### H4 – Retention

Prüfe die erworbene neue Beziehung anschließend ohne weitere Plastizität.

Eine Recovery-Variante, die eine bestehende Antwort stabilisiert, aber notwendiges Umlernen verhindert, ist nicht freigabefähig.

---

## 13. Phase I – Kausale Gegenproben

Bei relevanten Verbesserungen oder Verschlechterungen werden vollständige Zustandskopien eingesetzt.

### I1 – Gleicher Refraktärzustand, andere Relaxation

Vergleiche R1 und R2 bei identischer Konfiguration und identischem Ausgangszustand.

Damit wird die tatsächliche Wirkung zusätzlicher Membranrelaxation isoliert.

### I2 – Gleiche Relaxation, unterschiedliche Verzögerung

Prüfe, ob der Zeitpunkt des Recoveryereignisses die autonome Entscheidung beeinflusst.

### I3 – Recovery vor und nach einem Reiz

Vergleiche:

- Recovery bereits ausgeführt.
- Recovery noch ausstehend.
- Recovery veraltet.
- Kein Recoveryereignis vorhanden.

### I4 – Sensorische und inhibitorische Wechselwirkung

Prüfe, ob eine veränderte Membrandynamik eine zuvor bestehende Konkurrenzsituation auflöst oder lediglich verschiebt.

### I5 – Beide Beziehungen

Jede relevante kausale Verbesserung muss auch hinsichtlich der anderen Reizbeziehung untersucht werden.

### I6 – Keine Erfolgsselektion

Die Auswahl diagnostischer Fälle muss vorab festgelegt sein.

Es dürfen nicht ausschließlich diejenigen Zustände untersucht werden, in denen Recovery bereits erfolgreich war.

---

## 14. Kontrollgruppen

Mindestens folgende Gruppen sind vorzusehen:

| Gruppe | Inhalt |
|---|---|
| K0 | Unveränderte B.5.3-Referenz |
| K1 | No-Learning |
| K2 | Feste inhibitorische Wirkung |
| K3 | Lernbare inhibitorische Wirkung |
| K4 | R0 mit kontrolliertem Refraktärwert |
| K5 | R1: unabhängige Recovery ohne Relaxation |
| K6 | R2: unabhängige Recovery mit 0 % Relaxation |
| K7 | R2: unterschiedliche Relaxationsstärken |
| K8 | Unterschiedliche Recovery-Verzögerungen |
| K9 | Diagnostische Recovery-Zustandskopien |
| K10 | Zeitliche Integrationskontrollen |
| K11 | Umweltumkehr und Retention |
| K12 | Gegebenenfalls begründete minimale Forschungsvariante |
| K13 | Abschaltkontrolle einer neu eingeführten Funktion |

K12 und K13 werden nur benötigt, falls die Untersuchung wider Erwarten eine neue lokale Funktion notwendig macht und ihr kausaler Bedarf zuvor nachgewiesen wurde.

Die primäre Forschung soll die bereits implementierten Recovery-Modi verwenden.

---

## 15. Statistische Versuchsgestaltung

### Entdeckung

Mindestens:

- 64 gepaarte Seeds.
- Drei unabhängige Zufallsstromfamilien.
- Normale und inverse Zielzuordnung.

### Bestätigung

Mindestens weitere 64 unabhängige, zuvor nicht für die Auswahl verwendete Seeds je zentraler Kombination.

### Stufenweiser Versuchsplan

Die Untersuchung wird in drei Stufen durchgeführt:

**Stufe 1 – Technische Aktivierung**

Kleine deterministische Netze und gezielte Zustandskopien.

**Stufe 2 – Begrenzte Entdeckungsmatrix**

Vorab festgelegte, tatsächlich wirksame Recovery-Varianten mit gepaarten Seeds.

**Stufe 3 – Unabhängige Bestätigung**

Die aus der Entdeckung begründet ausgewählten Varianten werden unverändert auf den Bestätigungs-Seeds untersucht.

Falls keine Variante die mechanistischen Grundanforderungen besteht, entfällt eine umfangreiche Bestätigung wirkungsloser Parameterkombinationen. Der negative Befund wird dokumentiert.

### Auswertung

Erforderlich sind:

- Gepaarte Unterschiede.
- Seedbasierte Erfolgsquoten.
- Clusterbewusste 95-%-Bootstrap-Konfidenzintervalle.
- E1- und E2-Verwechslungsmatrizen.
- Retentionsverläufe.
- Zeiten bis zum ersten Erwerb.
- Zeiten bis zum Verlust.
- Gemeinsame Abrufbarkeit.
- Tatsächlich ausgeführte Recoveryereignisse.
- Relaxationswirkung.
- Refraktärbedingte Feuersperren.
- Ressourcenkosten.

---

## 16. Allgemeine Erfolgskriterien

Eine neue Recovery-Konfiguration darf nur dann als allgemein erfolgreich gelten, wenn die bekannten Kriterien erfüllt werden:

- Mindestens 75 % E1 für X.
- Mindestens 75 % E1 für Y.
- Mindestens 48/64 Seeds erfüllen beide Schwellen gleichzeitig.
- Mindestens 15 Prozentpunkte gepaarter Vorteil gegenüber No-Learning.
- Vollständig positives 95-%-Konfidenzintervall.
- Reproduktion auf unabhängigen Seeds.
- Erfolg bei normaler und inverser Zuordnung.
- Reproduktion über drei unabhängige Stromfamilien.
- Retention ohne weitere Plastizität.
- Erhalt der Umweltanpassungsfähigkeit.

### Zusätzliche Recovery-Anforderungen

Eine Variante muss außerdem:

1. Eine tatsächlich aktive lokale Recovery besitzen.
2. Einen isolierbaren Beitrag der Membranrelaxation zeigen.
3. Die notwendige zeitliche Integration erhalten.
4. Beide Reizbeziehungen gemeinsam berücksichtigen.
5. Keine technische Auswahl richtiger Antworten verwenden.
6. Keine unkontrollierten Recovery-Ereignisschleifen verursachen.
7. Persistenz und deterministische Verarbeitung erhalten.

### Wissenschaftlicher Teilerfolg

B.5.4 darf auch mit negativem Ergebnis abgeschlossen werden.

Ein wissenschaftlich wertvoller Befund kann beispielsweise sein, dass:

- Die vorhandene Recovery technisch nicht ausreichend wirksam wird.
- Die Refraktärwirkung die Lernleistung dominiert.
- Membranrelaxation zeitliche Integration zerstört.
- Die Verbesserung eines Pfads die andere Beziehung verschlechtert.
- Die vorhandene Recovery ausschließlich nach Feuern greift und damit relevante subthreshold-Zustände nicht erreicht.

Es dürfen keine positiven Schlussfolgerungen ohne isolierte kausale Evidenz gezogen werden.

---

## 17. Architekturgrenzen

Verbindlich bleiben:

- C++17.
- Ereignisgesteuerte Verarbeitung.
- Kein globaler neuronaler Tick.
- Binäre Feuerereignisse.
- Lokal gewichtete synaptische Eingänge.
- Erregende und hemmende Synapsen.
- G3 und lokale Eligibility.
- Drei-Faktor-Plastizität.
- Geschützte angeborene Exploration.
- Keine externe richtige Handlungskennung im neuronalen System.
- Keine globale Motorselektion.
- Kein globaler Membranreset.
- Kein automatisches Wiederherstellen früher erfolgreicher Zustände.
- Kein pauschaler Gewichtsschutz.
- Keine permanenten Zeitstempel pro Synapse.
- Keine technische Bevorzugung bestimmter Sensor- oder Motorrollen.
- Vollständige Persistenz.
- Deterministisch reproduzierbare Ereignisverarbeitung.

Synapsenwachstum bleibt in den primären Untersuchungen deaktiviert.

### Wichtig

Eine veränderte Refraktärdauer oder Membranrelaxation ist eine kontrollierte neuronale Forschungsvariante und darf nicht als unveränderte historische Referenz ausgegeben werden.

---

## 18. Technische Validierung

Alle bisherigen 543 Kerntests müssen bestehen bleiben.

Zusätzliche Prüfungen sind erforderlich für:

- Tatsächlichen Reset-Modus der Referenz.
- R0 ohne Recoveryereignis.
- R1 mit null Refraktärereignissen.
- R2 mit null Refraktärereignissen.
- R1 und R2 mit aktivem Refraktärzustand.
- Tatsächlich wirksame Relaxation.
- R2 bei 0 % Relaxation.
- R2 bei 100 % Relaxation.
- Recovery-Verzögerung und Ereignisreihenfolge.
- Veraltete Recovery-Generationen.
- Wiederholtes Feuern vor Recovery.
- Recovery nach inhibitorischer Aktivität.
- Subthreshold-Restpotenziale.
- Temporale Integration.
- Zwei gleichzeitig erlernte Beziehungen.
- Geschützte Exploration.
- Umweltumkehr.
- Persistenz ausstehender Recoveryereignisse.
- Deterministische Fortsetzung.
- Gültigkeit neuronaler Zustände.
- Begrenzte Event-Queue und Ressourcenverwendung.

Vorhandene Tests dürfen nicht lediglich an geänderte Ergebniswerte angepasst werden, um Fehler verschwinden zu lassen.

---

## 19. Ressourcen und Persistenz

Für jede Variante erfassen:

- Anzahl geplanter Recoveryereignisse.
- Anzahl tatsächlich durchgeführter Recoveryereignisse.
- Anzahl veralteter Recoveryereignisse.
- Maximale Warteschlangenlänge.
- Speicherbedarf.
- Ereignisanzahl je Episode.
- Laufzeit.
- Zusätzliche Verarbeitungsarbeit durch Recovery.

Eine Verbesserung der neuronalen Trefferquote bei unkontrollierter Ereignisvermehrung ist nicht ausreichend.

Die Forschungsrichtung soll langfristig auch ressourcenbeschränkte Hardware ermöglichen.

Daher ist die Effizienz lokaler Ereignisverarbeitung ein eigenständiges Bewertungskriterium.

### Persistenz

Sämtliche Recovery-Konfigurationen und ausstehenden Ereignisse müssen speicherbar und exakt wiederherstellbar bleiben.

Testen:

- Speichern vor einem Recoveryereignis.
- Laden vor seiner Fälligkeit.
- Deterministisches Eintreten nach Wiederherstellung.
- Veraltete Ereignisse nach neuem Feuern.
- Identische Membranpotenziale und Antworten.

---

## 20. Lieferumfang

Nova-EVE erstellt:

1. Technisches Audit der vorhandenen Recovery-Implementierung.
2. Dokumentation und Klärung der Reset-Konfiguration.
3. Prüfung der Refraktärabhängigkeit vorhandener Recoverymodi.
4. Deterministische Recovery-Mikroexperimente.
5. Vergleich R0, R1 und R2.
6. Vorab definierte Parameter- und Kontrollmatrix.
7. Vollständige lokale Ereignisprotokolle.
8. Analyse zeitlicher Integration.
9. Natürliche gemischte Lernverläufe.
10. Untersuchung beider gleichzeitig erlernter Beziehungen.
11. Retentions- und Umweltumkehrtests.
12. Gepaarte kausale Zustandskopien.
13. Unabhängige Bestätigungsversuche.
14. Ressourcen- und Persistenzprüfung.
15. Regressionstests.
16. Vollständige Rohdaten und statistische Auswertung.
17. Forschungsbericht B.5.4.
18. Technische Recovery-Analyse.
19. Entscheidungsvorlage für B.5.5.

Historische Forschungsdaten bleiben unverändert.

---

## 21. Abschlussfragen

Am Ende muss beantwortet werden:

**A.** Welcher Reset-Modus wird tatsächlich in der bisherigen Forschung verwendet?

**B.** Unter welchen Bedingungen werden die vorhandenen Recovery-Modi wirksam?

**C.** Welche Wirkung besitzt ein Refraktärzustand unabhängig von zusätzlicher Membranrelaxation?

**D.** Kann lokale Membranrelaxation die Zahl transienter Abrufstörungen reduzieren?

**E.** Welche Relaxationsstärken und Zeitabstände erhalten die zeitliche Integration?

**F.** Kann Recovery die gemeinsame Verfügbarkeit zweier erlernter Beziehungen verbessern?

**G.** Wird eine verbesserte Leistung durch Relaxation oder lediglich durch allgemeine Feuersperren erzeugt?

**H.** Beeinflusst die veränderte Membrandynamik die lokale Eligibility und das spätere Lernen?

**I.** Bleibt die geschützte Exploration funktionsfähig?

**J.** Bleibt Umlernen nach veränderter Umweltbewertung möglich?

**K.** Welche Restpotenzialzustände werden von der vorhandenen Recovery grundsätzlich nicht erreicht?

**L.** Ist die vorhandene lokale Recovery ausreichend, oder weist sie einen konkreten, kausal nachgewiesenen funktionalen Mangel auf?

**M.** Welche zusätzliche Forschungsfrage ergibt sich daraus für B.5.5?

Eine neue allgemeine neuronale Recovery-Funktion darf nur nach reproduzierbarer kausaler Untersuchung freigegeben werden.

---

## 22. Forschungsleitlinie

**Neuronale Restaktivität ist nicht grundsätzlich ein Fehler.**

Ein Neuron muss frühere Eingänge berücksichtigen können, damit zeitliche Integration und daraus entstehende Entscheidungen möglich sind.

Gleichzeitig darf eine vergangene Aktivität das Netzwerk nicht unbegrenzt in einer bestimmten Erregungslage festhalten, wenn dadurch neue Erfahrungen nicht mehr angemessen verarbeitet werden.

Für EVE suchen wir deshalb keine technisch richtige Membranspannung.

Wir suchen eine lokale Dynamik, die:

- Vergangene Aktivität sinnvoll berücksichtigt.
- Neue Eingänge verarbeitet.
- Vorübergehende Erregung begrenzen kann.
- Unterschiedliche Reiz-Handlungs-Beziehungen verfügbar hält.
- Weiteres Lernen und Umlernen ermöglicht.
- Ohne globale Steuerung auskommt.
- Mit ereignisgesteuerter, ressourceneffizienter Verarbeitung vereinbar bleibt.

Eine Recovery-Regel darf weder wissen, welche Handlung richtig ist, noch einen früheren erfolgreichen Zustand künstlich wiederherstellen.

**Das Ziel von B.5.4 ist, experimentell zu bestimmen, ob und wie die bereits vorhandene lokale Recovery-Architektur neuronale Entscheidungen stabilisieren kann – und welche Eigenschaften einer natürlichen Membrandynamik dabei unverzichtbar sind.**
