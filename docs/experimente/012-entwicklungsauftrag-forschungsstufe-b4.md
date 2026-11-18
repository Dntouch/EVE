# EVE – Forschungsstufe B.4
## Erste erfahrungsabhängige synaptische Plastizität
### Biologisch inspiriertes Drei-Faktor-Lernen ohne eindeutige Handlungsidentifikation

**Auftrag an:** Nova-EVE  
**Projekt:** EVE – Emergent Virtual Evolution  
**Repository:** `Dntouch/EVE`  
**Ausgangsbasis:** `codex/research-stage-b31-neural-action-context`  
**Status:** Neuer Forschungs- und Entwicklungsauftrag  
**Implementierung:** C++17

---

# 1. Grundsätzliche Forschungsentscheidung

Mit Forschungsstufe B.4 wird die bisherige Entwicklungsrichtung bewusst angepasst.

Die Forschungsstufen B.3 und B.3.1 versuchten, neuronale Rückmeldungen eindeutig den zuvor ausgeführten Handlungen zuzuordnen.

Das erwies sich als schwierig.

B.3.1 erreichte bei zwei konkurrierenden Handlungen:

- 3,57 % eindeutig richtige Zuordnungen.
- 0,67 % eindeutig falsche Zuordnungen.
- 53,83 % mehrdeutige Zuordnungen.
- 41,93 % nicht zuordenbare Rückmeldungen.

Die bisherige Architekturentscheidung 0011 sperrte deshalb den Übergang zu rückmeldungsabhängigen Gewichtsänderungen.

Diese Entscheidung wird **für eine ausdrücklich begrenzte, experimentelle Forschungsstufe** neu bewertet.

Wir verfolgen nun einen anderen Ansatz.

Das biologische Gehirn benötigt für einfache synaptische Lernvorgänge nicht zwingend eine eindeutige Identifikation jeder einzelnen vorausgegangenen Handlung.

Stattdessen können zeitlich begrenzte lokale Aktivitätsspuren zusammen mit einem modulierenden Rückmeldungssignal Veränderungen synaptischer Verbindungen ermöglichen.

Dieses Prinzip wird als *Drei-Faktor-Lernen* beziehungsweise *neuromodulierte synaptische Plastizität* bezeichnet.

## Neue Forschungsentscheidung

**Die vollständige Handlungsidentifikation wird nicht länger als Voraussetzung für den ersten experimentellen Lernversuch verlangt.**

Stattdessen wird untersucht, ob EVE durch das Zusammenspiel aus:

1. präsynaptischer Aktivität,
2. postsynaptischer Aktivität beziehungsweise lokalem neuronalen Zustand,
3. modulierender Rückmeldung

ihre synaptischen Verbindungen so verändern kann, dass sich ihr Verhalten durch Erfahrungen verbessert.

**Erstmals werden damit rückmeldungsabhängige Gewichtsänderungen in einem isolierten Forschungsaufbau zugelassen.**

Die bestehende Entscheidung 0011 bleibt als Dokumentation der unzureichenden Handlungsidentifikation bestehen. Eine neue Architekturentscheidung muss die begrenzte experimentelle Freigabe und ihre Sicherheitsgrenzen ausdrücklich dokumentieren.

Eine Freigabe für den produktiven EVE-Betrieb ist damit nicht verbunden.

---

# 2. Zentrale Forschungsfrage

**Kann EVE aus wiederholten eigenen Erfahrungen lernen, indem lokal aktive neuronale Verbindungen vorübergehend lernberechtigt werden und ein später eintreffendes modulierendes Signal ihre Gewichtungen verändert?**

Der Nachweis muss durch eine messbare Veränderung des Verhaltens erfolgen.

Eine veränderte Synapsengewichtung allein ist kein ausreichender Lernerfolg.

Ebenso ist eine gestiegene allgemeine neuronale Aktivität kein ausreichender Nachweis.

Als erstes Ziel soll EVE lernen, zwischen zwei möglichen Handlungen diejenige häufiger auszuführen, die in ihrer Umwelt zu einer besseren Rückmeldung führt.

Die richtige Handlung wird nicht im neuronalen Netzwerk vorgegeben.

---

# 3. Bestehende Architektur

Die bisherige Architektur bleibt grundsätzlich erhalten.

## BIOS

Zuständig für:

- Technische Ein- und Ausgabe.
- Ereignisplanung.
- Simulierte Zeit.
- Persistenz.
- Beobachtung tatsächlicher motorischer Ausgaben.
- Schnittstelle zur Versuchsumgebung.
- Unabhängige Forschungsdiagnose.

## Stammhirn

Zuständig für:

- Angeborene sensorische und motorische Funktionen.
- Lokale neuronale Lernmechanismen.
- Erzeugung und Verfall von Lernspuren.
- Verarbeitung modulierender neuronaler Signale.
- Begrenzungs- und Regulationsmechanismen.

## Neuronales Netzwerk

Zuständig für:

- Verarbeitung neuronaler Impulse.
- Synaptische Gewichtungen.
- Membranzustände.
- Lokale Ereignisgedächtnisse.
- Kurzfristige synaptische Lernspuren.
- Tatsächliche synaptische Anpassungen.

## Versuchsumgebung

Die Umgebung stellt Aufgaben und beobachtbare Konsequenzen bereit.

Sie darf eine Handlung bewerten.

Sie darf jedoch niemals:

- Die richtige Handlung direkt im Netzwerk aktivieren.
- Technisch bekannte beteiligte Synapsen auswählen.
- Den korrekten Lösungsweg vorgeben.
- Einzelne Synapsengewichte aufgrund ihrer technischen Diagnose ändern.
- Eine versteckte Handlungs-ID als neuronale Lernhilfe verwenden.

---

# 4. Verbindliche technische Grundlagen

Die bisherige sparsame, ereignisgesteuerte Architektur bleibt bestehen.

Verbindlich sind:

- Kein globaler neuronaler Tick.
- Keine regelmäßigen vollständigen Netzwerkdurchläufe.
- Keine permanenten Berechnungen inaktiver Neuronen.
- Keine Zeitstempel pro Synapse.
- Keine globalen Handlungs-IDs im neuronalen Zustand.
- Keine Nutzung technischer M3-Kausalitätslisten für Gewichtsänderungen.
- Keine vorgegebenen semantischen Bedeutungen.
- Keine künstliche Wiederherstellung des Netzwerkzustands zwischen Trainingserfahrungen.

Eine Synapse besitzt weiterhin individuelle Gewichtungen.

Neuronale Ausgänge bleiben logisch binäre Feuerereignisse.

Die bisherige Trennung zwischen Membranpotenzial, Refraktärzustand, G3, Eligibility und synaptischem Gewicht wird beibehalten.

## 4.1 Kontextsignaturen aus B.3.1

Die bisher eingeführten 32-Bit-Kontextsignaturen dürfen nicht als Voraussetzung des Lernverfahrens verwendet werden.

Sie stellen einen zusätzlichen Informationskanal dar, der über das einfache binäre Spike-Prinzip hinausgeht.

Für die primären B.4-Versuche sind diese Kontextmechanismen zu deaktivieren.

Sie dürfen als separate technische Vergleichsvariante erhalten bleiben, aber nicht stillschweigend in die Lernregel einfließen.

---

# 5. Forschungsphase I – Minimale Lernumgebung

## 5.1 Ziel

Schaffe eine kleine, reproduzierbare Umgebung, in der EVE durch eigene neuronale Entscheidungen zwischen zwei möglichen Handlungen wählen kann.

Die Umgebung muss einfach genug sein, um einen Lernerfolg eindeutig beobachten zu können.

## 5.2 Zwei mögliche Handlungen

Definiere zwei motorische Ausgabemöglichkeiten:

- **Handlung A**
- **Handlung B**

Diese repräsentieren ausschließlich zwei unterscheidbare motorische Ausgänge.

Sie besitzen keine vorgegebene semantische Bedeutung.

Die Auswahl muss durch tatsächliche neuronale Aktivität erfolgen.

Das BIOS darf nicht per Zufallsfunktion A oder B auswählen und anschließend behaupten, EVE habe sich entschieden.

## 5.3 Erkennung einer Handlung

Eine Handlung gilt als ausgeführt, wenn der entsprechende motorische Ausgang aufgrund regulärer neuronaler Aktivität ausgelöst wurde.

Es sind folgende Ergebnisse zu unterscheiden:

- Nur A.
- Nur B.
- A und B innerhalb desselben Entscheidungsfensters.
- Keine Handlung.

Für gleichzeitige oder fehlende Entscheidungen ist eine vorab definierte, neutrale Auswertungsregel festzulegen.

Solche Fälle dürfen nicht aus der Statistik verschwinden.

## 5.4 Umweltbewertung

Die Umgebung besitzt eine einfache Erfolgsregel.

Beispiel:

- A führt zur positiven Konsequenz.
- B führt zur negativen oder ausbleibenden positiven Konsequenz.

Die Zuordnung ist ausschließlich der Umgebung bekannt.

In weiteren Versuchsreihen wird sie umgekehrt.

Die Umgebung darf nur das tatsächlich beobachtete Ergebnis bewerten.

## 5.5 Keine vorgegebene Lösung

Das Netzwerk darf nicht so verdrahtet werden, dass bereits die angeborene Schaltung die bevorzugte Handlung auswählt.

Unterschiedliche motorische Ausgänge dürfen technisch definiert sein.

**Welche Handlung EVE bevorzugt, soll sich jedoch aus neuronaler Aktivität und späteren Erfahrungen ergeben.**

## 5.6 Vorversuch zur Handlungsfähigkeit

Vor jedem Lernexperiment ist zu prüfen:

- Können grundsätzlich beide motorischen Ausgänge aktiviert werden?
- Wie häufig entstehen A, B, beide oder keine Handlung?
- Besteht bereits eine starke Ausgangspräferenz?
- Ist ausreichende Exploration vorhanden?
- Sind beide Ausgänge unter vergleichbaren Bedingungen erreichbar?

Wenn das Netzwerk nur A erzeugen kann, darf eine spätere Bevorzugung von A nicht als Lernen bewertet werden.

Alle Seeds sind zu dokumentieren, auch solche ohne ausreichende Handlungsfähigkeit.

Eine gesonderte Gruppe mit nachgewiesener anfänglicher Erreichbarkeit beider Ausgänge ist zulässig, muss aber klar von der vollständigen Seed-Stichprobe getrennt bleiben.

---

# 6. Forschungsphase II – Lokale synaptische Lernspuren

## 6.1 Ausgangssituation

Die bisherige Eligibility-Regel verwendet das lokale G3-Ereignisgedächtnis.

Dabei werden mögliche beteiligte Synapsen bei einer Feuerentscheidung markiert.

B.2 zeigte, dass bereits zwei Kandidaten viele relevante M3-Verbindungen erfassen können.

Die Selektivität war allerdings begrenzt.

B.4 untersucht daher eine biologisch stärker motivierte Lernspur.

## 6.2 Drei-Faktor-Grundprinzip

Ein synaptischer Lernkandidat entsteht aus dem Zusammenspiel von:

**Faktor 1 – Präsynaptische Aktivität**

Ein Impuls erreicht das empfangende Neuron über eine konkrete Synapse.

**Faktor 2 – Postsynaptische Aktivität**

Das empfangende Neuron zeigt eine relevante lokale Reaktion.

Diese kann zunächst als tatsächliches Feuern definiert werden.

Weitere lokale Aktivitätskriterien sind experimentell zu vergleichen.

**Faktor 3 – Modulierende Rückmeldung**

Ein später eintreffendes modulierendes Signal kann die noch gültige synaptische Lernspur in eine Gewichtsänderung umsetzen.

Die ersten beiden Faktoren erzeugen nur eine vorübergehende Lernspur.

**Ohne dritten Faktor darf daraus in der neuen Lernregel keine dauerhafte Gewichtsänderung entstehen.**

## 6.3 Zu vergleichende Varianten

### E0 – Keine Lernspur

Referenz ohne neue Eligibility.

### E1 – Bestehende G3-Eligibility

Die bisherige L1-Auswahl wird als Ausgangsvariante verwendet.

Sie kann auch Synapsen mit Gewicht 0 berücksichtigen.

### E2 – Präsynaptische Aktivität und postsynaptisches Feuern

Eine Synapse wird nur dann markiert, wenn sie im relevanten lokalen Aktivitätsfenster Impulse übertragen hat und das empfangende Neuron eine Feuerentscheidung ausführt.

### E3 – Präsynaptische Aktivität und lokale Membranbeteiligung

Untersuche ergänzend, ob eine Lernspur bereits durch einen lokalen Beitrag zur neuronalen Erregung entstehen kann, ohne dass für jede Markierung ein neues Feuerereignis erforderlich ist.

Dies ist eine eigenständige experimentelle Variante.

## 6.4 Nullgewicht-Synapsen

Ein besonders wichtiger Bestandteil des bisherigen EVE-Konzepts:

**Neue Synapsen beginnen mit Gewicht 0.**

Diese Verbindungen müssen die Möglichkeit behalten, später wirksam zu werden.

Eine Lernregel, die ausschließlich bereits wirksame Synapsen berücksichtigt, kann keine ursprünglich unwirksame Verbindung aufbauen.

Deshalb sind Nullgewicht-Synapsen ausdrücklich zu untersuchen.

Die Aktivität über eine Nullgewicht-Synapse darf lokal registriert werden.

Wenn das empfangende Neuron durch andere Eingänge eine relevante Reaktion zeigt, kann auch diese zuvor aktive Nullgewicht-Synapse einen möglichen Lernkandidaten darstellen.

Das beweist keine kausale Beteiligung, eröffnet aber die Möglichkeit, bislang unwirksame Verbindungen zu verstärken.

## 6.5 Begrenzter Lernspurenspeicher

Untersuche mindestens:

- 2 Kandidaten.
- 4 Kandidaten.
- 8 Kandidaten.

Der Speicher bleibt lokal begrenzt.

Mehrfachaktivierungen derselben Synapse müssen definiert behandelt werden.

Ein globaler Speicher aller aktiven Synapsen ist nicht vorgesehen.

---

# 7. Forschungsphase III – Zeitlicher Verfall der Lernspuren

## 7.1 Ziel

Eine Lernspur soll nicht unbegrenzt gültig bleiben.

Frühere Aktivität soll mit zunehmendem zeitlichen Abstand weniger Einfluss auf eine spätere Rückmeldung besitzen.

## 7.2 Versuchsvarianten

### T0 – Unmittelbare Lernberechtigung

Nur eine unmittelbar verfügbare Lernspur ist gültig.

### T1 – Konstante kurzfristige Berechtigung

Die Spur bleibt für eine begrenzte Dauer vollständig gültig und verfällt anschließend.

### T2 – Stufenweise abnehmende Berechtigung

Die Spur besitzt eine kleine diskrete Stärke, die über mehrere Ereigniszeitpunkte abnimmt.

Beispielhaft:

- Frische Spur: Stärke 4.
- Erste Alterungsstufe: Stärke 2.
- Zweite Alterungsstufe: Stärke 1.
- Danach: ungültig.

Die Werte dienen ausschließlich als Versuchshypothese.

### T3 – Verfall bei weiteren lokalen Aktivitäten

Untersuche als Vergleich, ob die Lernspur zusätzlich durch nachfolgende neuronale Aktivitäten abgeschwächt oder überschrieben wird.

## 7.3 Zeitparameter

Vergleiche mindestens die bekannten abstrakten Dauern:

- 4 Simulationseinheiten.
- 16 Simulationseinheiten.
- 64 Simulationseinheiten.

## 7.4 Ereignisgesteuerte Umsetzung

Der Verfall muss ohne globalen Tick erfolgen.

Zulässig sind:

- Geplante lokale Ablaufereignisse.
- Kleine lokale Generationszähler.
- Zustandsänderungen bei tatsächlich eintreffenden Ereignissen.
- Berechnung des verbleibenden Zustands beim lokalen Zugriff.

Keine per Synapse ständig aktualisierten Zeitstempel.

Der Speicher- und Verwaltungsbedarf verschiedener Lösungen muss gemessen werden.

---

# 8. Forschungsphase IV – Modulierende neuronale Rückmeldung

## 8.1 Ziel

Führe ein vom normalen Erregungssignal unterscheidbares modulierendes Lernsignal ein.

Dieses soll sich an biologischen neuromodulierenden Mechanismen orientieren.

Es darf jedoch nicht als technische Anweisung verstanden werden, bestimmte bekannte Synapsen zu verändern.

## 8.2 Grundprinzip

Eine Rückmeldung enthält ausschließlich eine Bewertung der beobachteten Wirkung.

Sie enthält nicht:

- Die richtige motorische Ausgabe.
- Eine Liste beteiligter Neuronen.
- Eine Liste beteiligter Synapsen.
- Eine technische Handlungs-ID.
- Eine bereits berechnete Gewichtsänderung.

## 8.3 Modulationsvarianten

### M0 – Keine Modulation

Keine rückmeldungsabhängige Gewichtsänderung.

### M1 – Positive Modulation

Ein positives Bewertungssignal kann gültige lokale Lernspuren verstärken.

### M2 – Negative Modulation

Ein negatives Bewertungssignal kann gültige lokale Lernspuren abschwächen.

### M3 – Positive, negative und neutrale Modulation

Untersuche drei unterscheidbare Zustände:

- Positiv.
- Neutral.
- Negativ.

Eine ausbleibende Rückmeldung darf nicht automatisch einer negativen Bewertung gleichgesetzt werden.

## 8.4 Neuronale Übertragung

Die Modulation soll aus einem tatsächlich empfangenen sensorischen beziehungsweise neuromodulatorischen Ereignis entstehen.

Nutze die vorhandene B.3-Infrastruktur als Ausgangspunkt, untersuche aber eine eigenständige modulierende Verarbeitung.

Die Modulation darf biologisch inspiriert als eigener lokaler Wirkmechanismus umgesetzt werden.

Sie muss nicht als gewöhnlicher erregender Synapsenimpuls ausgeführt werden, sofern die unterschiedliche Rolle ausdrücklich dokumentiert und geprüft wird.

Wichtig:

**Ein modulierendes Signal verändert keine Synapse allein aufgrund ihrer technischen Identität, sondern ausschließlich aufgrund ihres lokalen gültigen Lernzustands.**

## 8.5 Keine versteckte globale Synapsenbearbeitung

Eine Bewertung darf keinen vollständigen Durchlauf über sämtliche Synapsen auslösen.

Untersuche eine begrenzte ereignisgesteuerte Verteilung, beispielsweise über allgemeine modulierende Relais und lokale Empfangsbereiche.

Ein empfangendes Neuron darf seine eigenen wenigen Lernkandidaten untersuchen.

Es darf nicht vom BIOS eine Teilnehmerliste erhalten.

## 8.6 Zwei getrennte Nachweise

Prüfe ausdrücklich:

1. Erreicht das modulierende Signal tatsächlich Neuronen mit gültigen Lernspuren?
2. Führt das lokale Zusammentreffen anschließend zu einer Gewichtsänderung?

Eine fehlende neuronale Erreichbarkeit darf nicht durch direkte technische Gewichtsänderung ausgeglichen werden.

Eine idealisierte Modulation darf höchstens als ausdrücklich gekennzeichnete technische Positivkontrolle dienen, nicht als Nachweis der regulären EVE-Architektur.

---

# 9. Forschungsphase V – Erste echte Gewichtsänderung

## 9.1 Grundregel

Eine synaptische Gewichtung darf durch den neuen Lernmechanismus nur dann verändert werden, wenn:

1. Eine gültige lokale Lernspur existiert.
2. Ein passendes modulierendes Signal tatsächlich eintrifft.
3. Die betreffende Synapse innerhalb der konfigurierten Lernregeln veränderbar ist.
4. Die geltenden Gewichts- und Regulationsgrenzen eingehalten werden.

## 9.2 Positive Veränderung

Bei positiver Modulation darf eine gültige Verbindung um einen begrenzten Betrag verstärkt werden.

Ein erster Versuch verwendet ganzzahlige Gewichtsschritte.

Beispiel:

`Gewicht 0 → Gewicht 1`

oder

`Gewicht 1 → Gewicht 2`

## 9.3 Negative Veränderung

Bei negativer Modulation darf eine gültige Verbindung abgeschwächt werden.

Beispiel:

`Gewicht 2 → Gewicht 1`

oder

`Gewicht 1 → Gewicht 0`

Negative Gewichtungen sind noch nicht vorgesehen.

Eine Verringerung auf 0 bedeutet noch keine physische Entfernung der Synapse.

## 9.4 Stärke der Veränderung

Untersuche zunächst:

- Einheitliche Gewichtsschritte.
- Gewichtsschritte abhängig von der verbliebenen Lernspurenstärke.
- Unterschiedliche Empfindlichkeit für positive und negative Modulation.

Die Versuchsvarianten sind unabhängig voneinander zu vergleichen.

Eine bestimmte Schrittweite darf nicht allein deshalb gewählt werden, weil sie zufällig ein erwünschtes Ergebnis liefert.

## 9.5 Gewichtsgrenzen

Vergleiche mindestens:

- Maximalgewicht 1.
- Maximalgewicht 2.
- Maximalgewicht 3.

Verwende vorhandene Eingangsgewichtbudgets und Konkurrenzmechanismen, soweit mit der neuen Regel vereinbar.

Die früheren Instabilitäten durch unkontrollierte Erregungsverstärkung müssen ausdrücklich berücksichtigt werden.

## 9.6 Schutzmaßnahmen

Erforderlich sind:

- Keine negativen Gewichte.
- Kein Überlauf.
- Keine Überschreitung konfigurierter Maximalgewichte.
- Keine Veränderung bei fehlender Modulation.
- Keine Veränderung durch veraltete Lernspuren.
- Keine rückwirkende Veränderung anhand technischer Diagnosen.
- Kein automatisches Synapsenwachstum durch Belohnung.
- Keine automatische Synapsenlöschung.

---

# 10. Forschungsphase VI – Erstes Zwei-Handlungs-Lernexperiment

## 10.1 Ziel

Prüfe, ob EVE durch wiederholte Erfahrungen zunehmend die erfolgreichere von zwei möglichen Handlungen bevorzugt.

## 10.2 Versuchsablauf

1. Ein reproduzierbarer Ausgangszustand wird initialisiert.
2. EVE erhält einen einfachen sensorischen Reiz.
3. Das Netzwerk verarbeitet den Reiz.
4. EVE erzeugt durch neuronale Aktivität eine motorische Ausgabe.
5. Die Versuchsumgebung beobachtet die tatsächliche Ausgabe.
6. Die Umgebung bewertet die Wirkung.
7. Die Bewertung wird neuronal als modulierendes Signal übertragen.
8. Gültige lokale Lernspuren können synaptische Änderungen auslösen.
9. EVE erhält erneut dieselbe Aufgabe.
10. Die Veränderung ihres Verhaltens wird gemessen.

## 10.3 Kein Zurücksetzen zwischen Versuchen

Während des Trainings bleiben erhalten:

- Synapsengewichte.
- Neuronale Verbindungen.
- Membranzustände.
- G3-Gedächtnisse.
- Refraktärzustände.
- Lernspuren entsprechend ihrer natürlichen Gültigkeit.
- Reguläre neuronale Zustände.

Nur die normale neuronale Erholung und der vorgesehene Verfall kurzlebiger Zustände finden statt.

Ein künstlicher Rücksprung zum ursprünglichen Netzwerkzustand zwischen zwei Erfahrungen ist nicht zulässig.

## 10.4 Trainingsumfang

Untersuche zunächst:

- 32 Erfahrungen.
- 128 Erfahrungen.
- 512 Erfahrungen.
- 2.048 Erfahrungen.

Bei instabiler Aktivität oder technischen Abbrüchen ist das zu dokumentieren.

Die Versuchsmatrix darf nach begründeten Vorversuchen erweitert werden.

## 10.5 Zentrale Messgröße

Die wichtigste Größe ist die Veränderung der Häufigkeit erfolgreicher neuronaler Handlungen.

Erfasse getrennt:

- Anteil Handlung A.
- Anteil Handlung B.
- Anteil beider Ausgänge gleichzeitig.
- Anteil ohne Ausgabe.
- Anteil positiver Bewertungen.
- Durchschnittliche Bewertung.
- Anzahl tatsächlicher Gewichtsänderungen.
- Zahl neu wirksam gewordener Nullgewicht-Synapsen.
- Zahl abgeschwächter Verbindungen.

**Eine hohe Zahl veränderter Synapsen zählt nicht als Lernerfolg, wenn sich die Handlungsqualität nicht verbessert.**

---

# 11. Forschungsphase VII – Verbindliche Lernkontrollen

Ein Lernerfolg darf nur gegenüber geeigneten Kontrollgruppen festgestellt werden.

## K0 – Kein neues Lernen

Neuronale Verarbeitung ohne neue Gewichtsänderungen.

## K1 – Aktivitätsabhängiges Lernen ohne Modulation

Die bisherige unmodulierte Aktivitätsverstärkung wird als getrennte Vergleichsvariante untersucht.

Sie darf nicht unbemerkt parallel zur neuen Lernregel laufen.

## K2 – Korrekte Modulation

Die tatsächliche Umweltbewertung wird neuronal übertragen.

## K3 – Zufällige Modulation

Die Bewertungen werden unabhängig vom tatsächlichen Handlungserfolg erzeugt.

Die Häufigkeiten und zeitlichen Eigenschaften der Bewertungssignale sollen möglichst vergleichbar bleiben.

## K4 – Vertauschte Bewertung

Die Umgebung belohnt gezielt die jeweils andere Handlung.

Damit wird untersucht, ob EVE die bevorzugte Handlung aufgrund der Erfahrung verändert.

## K5 – Zeitlich vertauschte Rückmeldung

Bewertungen werden gegenüber den tatsächlichen Handlungen zeitlich verschoben.

## K6 – Modulation ohne gültige Lernspur

Eine Rückmeldung trifft ein, obwohl keine gültige lokale Lernspur vorhanden ist.

Es darf keine neue Gewichtsänderung entstehen.

## K7 – Lernspur ohne Modulation

Lokale Aktivität erzeugt Lernspuren, aber keine modulierte Gewichtsänderung.

## K8 – Rückmeldung ohne Handlung

Eine Bewertung trifft ein, obwohl kein gültiger motorischer Ausgang stattgefunden hat.

## K9 – Rückmeldungsinduzierte Aktivität

Prüfe, ob durch die Rückmeldung selbst entstandene Lernspuren fälschlich für die zuvor bewertete Handlung verändert werden.

## K10 – Nullgewicht-Synapsen

Untersuche ausdrücklich, ob zuvor unwirksame Verbindungen eine erste Gewichtserhöhung erhalten können.

## K11 – Begrenzte synaptische Ressourcen

Untersuche Maximalgewichte und Eingangsgewichtbudgets.

## K12 – Keine Exploration

Prüfe, welche Änderungen ohne neue explorative Handlungsvarianten möglich sind.

## K13 – Identische Ausgangszustände

Alle Lern- und Kontrollgruppen beginnen aus identischen, reproduzierbaren Zuständen.

## K14 – Ausbleibende Rückmeldung

Eine Erfahrung bleibt ohne Bewertung.

## K15 – Wiederholte identische Eingaben

Untersuche, ob die Veränderung auf Erfahrung statt auf bloß wiederholte Stimulation zurückgeht.

Die Kontrollen müssen tatsächlich ausgeführt und mit Rohdaten dokumentiert werden.

---

# 12. Forschungsphase VIII – Veränderung der Umweltbedingungen

## 12.1 Ziel

Ein Netzwerk, das einmal zufällig eine bevorzugte Verbindung entwickelt, hat noch nicht notwendigerweise eine anpassungsfähige Lernfähigkeit.

Deshalb wird die Umwelt während eines fortgeführten Versuchs verändert.

## 12.2 Umkehr der Bewertung

Zunächst gilt beispielsweise:

- A wird positiv bewertet.
- B wird negativ bewertet.

Nach einer festgelegten Anzahl von Erfahrungen wird die Bewertung umgekehrt:

- A wird negativ bewertet.
- B wird positiv bewertet.

## 12.3 Forschungsfrage

**Kann EVE eine zuvor erworbene Präferenz aufgrund neuer Erfahrungen wieder verändern?**

## 12.4 Messgrößen

Erfasse:

- Häufigkeit der zuvor bevorzugten Handlung.
- Häufigkeit der neu bevorzugten Handlung.
- Anzahl Erfahrungen bis zu einer erkennbaren Anpassung.
- Veränderungen der synaptischen Gewichtungen.
- Auftreten neuronaler Instabilität.
- Mögliche vollständige Blockade einzelner Handlungswege.

Eine erfolgreiche Anpassung an die Umkehr wäre ein besonders aussagekräftiger Lernnachweis.

Eine dauerhaft unveränderliche Präferenz wäre dagegen ein Hinweis auf fehlende Anpassungsfähigkeit oder synaptische Sättigung.

---

# 13. Forschungsphase IX – Verzögerte und überlappende Belohnung

## 13.1 Ziel

Untersuche, ob Lernen ohne eindeutige Handlungsidentifikation auch bei verzögerten Rückmeldungen möglich ist.

Die bisherigen B.3.1-Befunde werden dabei nicht ignoriert.

Sie werden unter einer anderen Forschungsfrage betrachtet.

## 13.2 Zeitliche Varianten

Untersuche:

- Unmittelbare Bewertung.
- Bewertung nach 4 Simulationseinheiten.
- Bewertung nach 16 Simulationseinheiten.
- Bewertung nach 64 Simulationseinheiten.
- Bewertung nach Ablauf der jeweiligen Lernspur.

## 13.3 Überlappende Handlungen

Untersuche zusätzlich Folgen, bei denen eine weitere Handlung stattfindet, bevor die vorherige Bewertung eintrifft.

Hier kann es zu unpassenden Gewichtsänderungen kommen.

Diese werden nicht automatisch durch technische Handlungsidentifikation verhindert.

Stattdessen soll gemessen werden, wie sehr sie den tatsächlichen Lernfortschritt beeinflussen.

## 13.4 Wichtige Unterscheidung

Es sind zwei Fragen getrennt zu beantworten:

1. Wie viele lokal aktive Verbindungen werden vermutlich unpassend moduliert?
2. Kann das Netzwerk trotz dieser Unschärfe langfristig sein Verhalten verbessern?

Eine nicht perfekte Zuordnung bedeutet nicht automatisch, dass Lernen unmöglich ist.

Ein beobachteter Lernerfolg bedeutet umgekehrt nicht, dass das Zuordnungsproblem vollständig gelöst wurde.

---

# 14. Forschungsphase X – Stabilität und Selbstregulation

Frühere Forschungsstufen zeigten, dass unkontrollierte synaptische Verstärkung zu neuronaler Überaktivität führen kann.

Die Einführung rückmeldungsabhängiger Gewichtsänderungen darf dieses Problem nicht wieder unbemerkt auslösen.

## 14.1 Zu messen

- Synaptische Ereignisse pro Handlung.
- Feuerereignisse pro Handlung.
- Refraktär unterdrückte Feuerentscheidungen.
- Aktive und unwirksame Synapsen.
- Gewichtungsverteilung.
- Durchschnittliche Eingangsgewichtung.
- Maximale Eingangsgewichtung.
- Neuronen ohne Aktivität.
- Neuronen mit außergewöhnlich hoher Aktivität.
- Technische Ereignisabbrüche.
- Wiederholungsfähigkeit motorischer Handlungen.

## 14.2 Zu vergleichen

- Lernen ohne Gewichtsbudget.
- Lernen mit Gewichtsbudget.
- Positive Modulation allein.
- Positive und negative Modulation.
- Unterschiedliche Lernspurenkapazitäten.
- Unterschiedliche Gültigkeitsdauern.
- Unterschiedliche Maximalgewichte.

Die bestehende R2F-Erholung bleibt aktiv.

Eine neue globale neuronale Rücksetzfunktion ist nicht zulässig.

## 14.3 Abbruch und Dokumentation

Falls eine Variante unkontrollierte Aktivität oder Ressourcenüberschreitungen erzeugt, darf der einzelne Versuch durch sein technisches Sicherheitslimit beendet werden.

Der Abbruch muss als negatives beziehungsweise nicht abgeschlossenes Ergebnis dokumentiert werden.

Er darf nicht aus der Erfolgsstatistik entfernt werden.

---

# 15. Forschungsphase XI – Nachweis tatsächlichen Lernens

Dies ist der wichtigste Abschnitt des gesamten Auftrags.

## 15.1 Unterscheidung zwischen Plastizität und Lernen

**Plastizität:** Synapsengewichte verändern sich.

**Lernen:** Das neuronale System verbessert durch Erfahrung nachweislich sein Verhalten bei einer Aufgabe.

Die erste Eigenschaft beweist nicht automatisch die zweite.

## 15.2 Vorab definierte Erfolgsmaßstäbe

Vor den abschließenden Versuchsreihen sind festzulegen:

- Primäre Verhaltensmetrik.
- Vergleich der frühen und späten Trainingsphase.
- Vergleich mit der Kontrollgruppe ohne Lernen.
- Vergleich mit zufälliger Modulation.
- Vergleich mit vertauschter Modulation.
- Mindestanzahl gültiger Versuche.
- Verfahren zur statistischen Auswertung.

Als primärer Endpunkt wird die Erfolgsquote bei tatsächlichen neuronalen Handlungsversuchen empfohlen.

Fehlende und doppelte Ausgaben müssen getrennt ausgewiesen und zusätzlich in eine Gesamterfolgsmetrik einbezogen werden.

## 15.3 Training und unabhängige Überprüfung

Unterscheide zwischen:

**Trainingsphase**

EVE erhält Rückmeldungen und darf ihre Gewichte verändern.

**Überprüfungsphase**

Die erlernten Gewichte bleiben erhalten, aber die neue Plastizität wird vorübergehend deaktiviert.

Damit kann geprüft werden, ob sich das zuvor Erlernte tatsächlich im Verhalten zeigt.

Ein dafür gegebenenfalls verwendeter Zustandsabzweig dient ausschließlich als Messkopie.

Innerhalb eines fortgeführten Trainingsverlaufs darf kein Rücksetzen stattfinden.

## 15.4 Keine Erfolgsselektion

Es dürfen nicht nur die erfolgreichsten Seeds berichtet werden.

Die vollständige Verteilung ist auszuweisen.

Wenn nur einzelne Seeds lernen, die meisten aber nicht, muss dies ausdrücklich erkennbar bleiben.

## 15.5 Statistik

Verwende mindestens 64 reproduzierbare Ausgangsseeds.

Vergleiche Lern- und Kontrollgruppen gepaart auf identischen Anfangszuständen.

Berichte:

- Mittelwert.
- Median.
- Streuung.
- Anteil tatsächlich lernender Seeds.
- Verlauf der Erfolgsquote.
- Veränderung gegenüber dem Ausgangsniveau.
- Unsicherheitsintervalle der Unterschiede.

Eine einzelne besonders erfolgreiche Versuchskonfiguration reicht nicht für eine allgemeine Erfolgsbehauptung.

## 15.6 Entscheidender Erfolgstest

Eine Variante ist besonders interessant, wenn:

1. Die Erfolgsquote im Training steigt.
2. Die Verbesserung bei deaktivierter Plastizität erhalten bleibt.
3. Die Verbesserung gegenüber den Kontrollgruppen nachweisbar ist.
4. Die Gewichte tatsächlich durch lokale Aktivität und Modulation entstanden sind.
5. Das Netzwerk unter vergleichbaren Bedingungen nach einer Umkehr der Umweltbewertung seine Präferenz wieder ändern kann.

Es ist nicht erforderlich, dass jeder Versuch erfolgreich ist.

Es ist erforderlich, dass eine belastbare Verbesserung durch Erfahrung nachgewiesen wird.

---

# 16. Forschungsphase XII – Ressourcenverbrauch

Die sparsame, ereignisgesteuerte Architektur ist weiterhin ein Kernziel.

## 16.1 Anforderungen

- Keine globalen Synapsenscans je Rückmeldung.
- Kein globaler neuronaler Tick.
- Keine permanenten Berechnungen inaktiver Neuronen.
- Kompakte lokale Lernspuren.
- Begrenzte Speicherkapazitäten.
- Keine unbegrenzt wachsende Ereignishistorie.
- Deterministischer Ablauf.
- Keine GPU-Abhängigkeit.

## 16.2 Messgrößen

Erfasse:

- Zusätzliche Byte pro Neuron.
- Zusätzliche Byte pro Lernspur.
- Zusätzliche Byte pro aktivem Modulationszustand.
- Zahl zusätzlich geplanter Ereignisse.
- Laufzeit je Lernentscheidung.
- Laufzeit je modulierender Rückmeldung.
- Zahl tatsächlich geänderter Synapsen.
- Ereignisse pro Sekunde.
- Maximale Ereigniswarteschlange.
- RAM-Bedarf.
- CPU-Zeit.

## 16.3 Skalierung

Führe Funktionsversuche zunächst mit 256 Neuronen durch.

Erstelle Speicher- und Aufwandsschätzungen für:

- 10.000 Neuronen.
- 100.000 Neuronen.
- 1.000.000 Neuronen.

Größere Netzwerke müssen nicht die gesamte Lernmatrix durchlaufen.

Falls sinnvoll, sind gesonderte skalierte Leistungsversuche durchzuführen.

Gemessene und extrapolierte Ergebnisse müssen unterscheidbar bleiben.

---

# 17. Persistenz und Wiederaufnahme

Synaptisches Lernen muss dauerhaft erhalten bleiben.

Die Persistenz ist entsprechend zu erweitern.

Zu speichern sind gegebenenfalls:

- Synaptische Gewichte.
- Lokale Lernspuren.
- Verbleibende Gültigkeitszustände.
- Ausstehende Ablaufereignisse.
- Modulierende Ereignisse und ihre Herkunft.
- Konfiguration der Plastizitätsregel.
- Reguläre neuronale Zustände.
- Deterministische Zufallszustände.

## Verbindliche Tests

1. Speichern vor der ersten Lernerfahrung.
2. Speichern nach einer positiven Gewichtsänderung.
3. Speichern nach einer negativen Gewichtsänderung.
4. Speichern bei gültiger Lernspur.
5. Speichern bei ausstehender Modulation.
6. Speichern während einer längeren Lernsequenz.
7. Fortsetzen nach Wiederherstellung.
8. Identische Ergebnisse bei identischem Seed und Ereignisablauf.
9. Keine doppelte Anwendung einer bereits verarbeiteten Modulation.
10. Keine nachträgliche Gewichtsänderung durch veraltete Ablaufereignisse.
11. Rückwärtskompatibilität zu früheren Persistenzformaten.

Eine Wiederherstellung darf keine bereits erlernten Gewichte verlieren.

---

# 18. Automatisierte Tests

Ergänze die bestehende Testsuite um mindestens folgende Prüfungen.

## Lokale Lernspuren

1. Keine Lernspur bei deaktiviertem Mechanismus.
2. Präsynaptische Aktivität wird korrekt erkannt.
3. Postsynaptische Aktivität wird korrekt berücksichtigt.
4. Nullgewicht-Synapsen können entsprechend der gewählten Regel markiert werden.
5. Keine unzulässige Markierung fremder Synapsen.
6. Begrenzte Kapazität.
7. Korrektes Überschreiben.
8. Korrekte Gültigkeitsdauer.
9. Eigenständiger Ablauf ohne weitere Eingangssignale.
10. Keine Gewichtsänderung durch Ablauf allein.

## Modulation

11. Positive Rückmeldung wird neuronal übertragen.
12. Negative Rückmeldung wird neuronal übertragen.
13. Neutrale Rückmeldung verändert kein Gewicht.
14. Ausbleibende Rückmeldung verändert durch die neue Regel kein Gewicht.
15. Modulation ohne lokale Lernspur verändert kein Gewicht.
16. Rückmeldung enthält keine technische Synapsenliste.
17. Rückmeldung enthält keine technische Handlungs-ID.
18. Rückmeldungsinduzierte Lernspuren werden korrekt getrennt.

## Gewichtsänderung

19. Positive Modulation kann gültige Lernkandidaten verstärken.
20. Negative Modulation kann gültige Lernkandidaten abschwächen.
21. Keine Änderung außerhalb der Gewichtsgrenzen.
22. Keine negativen Gewichte.
23. Keine Änderung durch abgelaufene Spuren.
24. Keine Veränderung nicht erreichter, unberechtigter Synapsen.
25. Nullgewicht-Synapsen können erstmals Gewicht 1 erhalten.
26. Kein unbeabsichtigtes Synapsenwachstum.
27. Keine automatische synaptische Löschung.
28. Keine doppelte Anwendung desselben lokalen Modulationsereignisses.

## Verhalten

29. Beide motorischen Ausgänge sind grundsätzlich erreichbar.
30. Handlungsentscheidungen entstehen neuronal.
31. Die Umweltbewertung verändert nicht direkt die Motorausgabe.
32. Training ohne Modulation bleibt unverändert gegenüber der Referenz.
33. Trainingsverläufe sind reproduzierbar.
34. Vertauschte Bewertungen werden korrekt verarbeitet.
35. Umweltumkehr ist reproduzierbar.
36. Fortgeführtes Training benötigt keine Zustandswiederherstellung.
37. R2F-Erholung bleibt funktionsfähig.
38. Sicherheitslimits und Abbrüche werden korrekt dokumentiert.

## Persistenz

39. Erlernte Gewichtung bleibt nach Laden erhalten.
40. Aktive Lernspuren bleiben nach Laden konsistent.
41. Ausstehende Modulation wird genau einmal verarbeitet.
42. Veraltete Ereignisse verändern keinen wiederhergestellten Zustand.

Alle bisherigen 173 B.3.1-Tests müssen ebenfalls erfolgreich bleiben.

---

# 19. Forschungsbericht und Rohdaten

Erstelle einen vollständigen Forschungsbericht mit folgenden Abschnitten:

## A. Ausgangslage und Architekturentscheidung

Begründung für den Wechsel von expliziter Handlungszuordnung zum Drei-Faktor-Lernen.

## B. Erste Lernumgebung

Motorische Ausgangsmöglichkeiten, Exploration und Umweltbewertung.

## C. Lokale Lernspuren

Vergleich E0 bis E3.

## D. Zeitlicher Verfall

Vergleich T0 bis T3.

## E. Modulierende Rückmeldung

Übertragung, lokale Wirkung und Erreichbarkeit.

## F. Synaptische Plastizität

Positive und negative Gewichtsänderungen.

## G. Nullgewicht-Synapsen

Nachweis, ob bisher unwirksame Verbindungen wirksam werden können.

## H. Zwei-Handlungs-Lernen

Veränderung der beobachteten Handlungshäufigkeiten.

## I. Kontrollgruppen

Vergleich mit deaktiviertem Lernen, zufälligen und vertauschten Rückmeldungen.

## J. Umweltumkehr

Anpassung an eine veränderte Erfolgsregel.

## K. Verzögerte Rückmeldungen

Einfluss der Verzögerung und überlappender Handlungen.

## L. Neuronale Stabilität

Überaktivität, Sättigung und Regulationsmechanismen.

## M. Ressourcenverbrauch

Speicher, Laufzeit und Skalierungsverhalten.

## N. Persistenz

Speichern und Fortsetzen erlernter Zustände.

## O. Abschlussbewertung

Nachweisbarer Lernerfolg oder nachvollziehbare Gründe für ein Scheitern.

Alle Schlussfolgerungen müssen auf tatsächlich ausgeführten Experimenten beruhen.

Vordefinierte CSV-Ergebniszeilen ohne zugrunde liegende Versuchsdurchführung sind nicht zulässig.

---

# 20. Lieferumfang

Erstelle im Repository:

1. Entwicklungs- und Forschungsdokumentation B.4.
2. Technische Spezifikation der Drei-Faktor-Plastizität.
3. Implementierung der lokalen Lernspuren.
4. Implementierung ihrer ereignisgesteuerten Alterung.
5. Implementierung modulierender Rückmeldungen.
6. Isoliert aktivierbare positive und negative Gewichtsänderungen.
7. Minimale Zwei-Handlungs-Lernumgebung.
8. Reproduzierbare Trainings- und Kontrollversuche.
9. Rohdaten der Lernverläufe.
10. Vergleich der Eligibility-Varianten.
11. Vergleich der Modulationsvarianten.
12. Vergleich unterschiedlicher Rückmeldungsverzögerungen.
13. Versuche zur Umkehr der Umweltbewertung.
14. Analyse neuronaler Instabilität.
15. Ressourcen- und Performancemessungen.
16. Persistenztests.
17. Vollständige Regressionstests.
18. Forschungsbericht mit eindeutiger Erfolgsbewertung.
19. Neue Architekturentscheidung zur begrenzten Forschungsfreigabe.
20. Entscheidungsvorlage für die weitere Entwicklung.

Vorhandene Dokumente, Quelltexte, Rohdaten und fremde Arbeitsbaumänderungen dürfen nicht unbeabsichtigt verändert oder entfernt werden.

---

# 21. Verbindliche Forschungsgrenzen

Nicht Bestandteil dieser Forschungsstufe sind:

- Vollständige Sprachentwicklung.
- Semantische Begriffsbildung.
- Lernen komplexer Handlungsfolgen.
- Eigenständige neuronale Erfolgserwartung.
- Vorhersagefehler als zusätzlicher Lernfaktor.
- Neue hemmende Synapsentypen.
- Umfangreiche STDP-Verfahren mit synaptischen Zeitstempeln.
- Automatische synaptische Löschung.
- Unbegrenztes Synapsenwachstum.
- Globale technische Handlungsidentifikation.
- Technische Rückwärtszuordnung zu M3-Kausalität.
- Produktive Chatto-Kommunikation.

Die vorhandene B.3.1-Kontextinfrastruktur darf bestehen bleiben.

Sie ist aber für die primären Lernversuche zu deaktivieren.

B.4 dient ausschließlich dem ersten überprüfbaren Nachweis erfahrungsabhängiger neuronaler Anpassung.

---

# 22. Abschlussfragen

Am Ende müssen folgende Fragen empirisch beantwortet werden:

1. Kann EVE aus eigener neuronaler Aktivität lokale synaptische Lernspuren bilden?
2. Können diese Spuren zeitlich begrenzt bestehen bleiben?
3. Können sie ohne globale Aktualisierung verfallen?
4. Kann ein modulierendes neuronales Signal gültige Lernspuren erreichen?
5. Können dadurch erstmals positive synaptische Gewichtsänderungen entstehen?
6. Können dadurch negative synaptische Gewichtsänderungen entstehen?
7. Können Nullgewicht-Synapsen durch Erfahrungen wirksam werden?
8. Bleibt die neuronale Aktivität unter diesen Bedingungen stabil?
9. Kann EVE durch wiederholte Erfahrungen zunehmend eine erfolgreichere Handlung auswählen?
10. Ist die Verbesserung gegenüber geeigneten Kontrollgruppen nachweisbar?
11. Bleibt eine erworbene Präferenz bei deaktivierter Plastizität erhalten?
12. Kann EVE ihre Präferenz nach einer Umkehr der Umweltbewertung verändern?
13. Welche Auswirkungen haben verzögerte und überlappende Rückmeldungen?
14. Wie viel Speicher und Rechenleistung benötigt die zusätzliche Plastizität?
15. Welche neuronalen Mechanismen fehlen für zuverlässigeres Lernen?

## Erfolgskriterium

**Der zentrale Erfolg von B.4 ist nicht eine veränderte synaptische Gewichtung.**

**Der zentrale Erfolg ist eine nachweisbare Verbesserung des neuronalen Verhaltens durch eigene Erfahrungen.**

Falls dies nicht gelingt, sind die Ergebnisse trotzdem vollständig zu dokumentieren.

Insbesondere ist zu unterscheiden, ob ein Scheitern durch:

- fehlende motorische Exploration,
- unzureichende neuronale Erreichbarkeit,
- ungeeignete Lernspuren,
- zu lange Rückmeldungsverzögerung,
- falsche Gewichtsänderungsregeln,
- synaptische Sättigung,
- oder allgemeine neuronale Instabilität

verursacht wurde.

---

# 23. Übergeordnetes Entwicklungsziel

Forschungsstufe B.4 ist der erste kontrollierte Versuch, einen vollständigen neuronalen Erfahrungskreislauf zu schließen:

**Wahrnehmung → neuronale Entscheidung → Handlung → beobachtete Konsequenz → modulierte synaptische Anpassung → verändertes zukünftiges Verhalten.**

Dieser Mechanismus soll ohne vortrainiertes Wissen, ohne vorgegebene Handlungslösung und ohne ein Sprachmodell funktionieren.

Das Stammhirn definiert die angeborenen Voraussetzungen.

Die Umwelt liefert die Konsequenzen.

**Die neuronalen Verbindungen müssen sich aufgrund dieser Erfahrungen selbst verändern.**

Unser langfristiges Ziel bleibt eine künstliche Intelligenz, die Wissen, Fähigkeiten und später Sprache aus eigener Erfahrung entwickelt.

---

## Zentraler Forschungsgrundsatz

**EVE muss nicht wissen, welche ihrer Synapsen eine richtige Entscheidung verursacht hat.**

Es genügt für diesen ersten Versuch, dass lokale neuronale Aktivität vorübergehende Lernspuren hinterlässt und eine spätere Rückmeldung diese beeinflussen kann.

Ob daraus Lernen entsteht, entscheidet nicht unsere Vorstellung davon, wie es funktionieren müsste.

**Das entscheidet ausschließlich EVEs nachweisbar verändertes Verhalten.**
