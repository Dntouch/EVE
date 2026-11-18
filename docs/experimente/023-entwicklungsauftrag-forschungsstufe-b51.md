# Entwicklungsauftrag 023 – Forschungsstufe B.5.1

## Natürliche Lernwiederholung und Stabilität sensorischer Bindungen

**Projekt:** EVE – Emergent Virtual Evolution  
**Ausgangsbasis:** `codex/research-stage-b50-local-credit-assignment`  
**Technologie:** C++17  
**Forschungsstatus:** Kausale Grundlagenforschung, keine vorab freigegebene neue Lernregel

---

## 1. Ausgangslage

EVE verfügt inzwischen über folgende experimentell untersuchte neuronale Fähigkeiten:

- Ereignisgesteuerte Neuronen mit binären Feuerereignissen.
- Gewichtete erregende und hemmende Synapsen.
- Lokales G3-Ereignisgedächtnis.
- Zeitlich begrenzte Eligibility-Spuren.
- Drei-Faktor-Plastizität.
- Geschützte angeborene Exploration.
- Lokale inhibitorische Konkurrenz.
- Persistente neuronale Zustände.

B.5.0 konnte die lokale Kreditzuordnung einer positiv bewerteten Explorationshandlung erstmals bis zu einer konkreten synaptischen Veränderung verfolgen.

### Erkenntnisse aus B.5.0

In 384 prospektiv ausgewählten, tatsächlich erfolgreichen ersten Explorationsereignissen:

- Erhielt die passende sensorische Synapse in 378 Fällen Eligibility und wurde von Gewicht 0 auf 1 verstärkt.
- Blieb nach dieser einzelnen positiven Erfahrung in allen 384 Fällen eine autonome Antwort aus.
- Erreichten 364/384 diagnostische Kopien nach einer zweiten wiederholten positiven Erfahrung einen korrekten autonomen Abruf.
- Erreichten nach drei Wiederholungen 384/384 diagnostische Kopien eine korrekte autonome Antwort.

Die untersuchte neuronale Feuergrenze beträgt 2. Ein sensorischer Pfad mit Gewicht 1 ist in diesem Versuchsaufbau allein noch nicht ausreichend wirksam.

Der Übergang von Gewicht 1 auf Gewicht 2 wurde in 364 erfolgreichen Zustandskopien als notwendig und hinreichend für den autonomen Abruf identifiziert.

### Methodische Einschränkung

Die positiven Wiederholungen in B.5.0 wurden diagnostisch mit derselben zuvor erfolgreichen Explorationshandlung durchgeführt.

Dabei wurde vor jeder weiteren positiven Modulation nicht ausdrücklich geprüft, ob die gesamte neu entstandene Motorantwort tatsächlich wieder korrekt und eindeutig war.

Diese Einschränkung muss in B.5.1 behoben werden.

Eine positive Bewertung darf nur auf eine tatsächlich beobachtete erfolgreiche Handlung folgen.

### Weiterhin offener Engpass

Trotz funktionierender lokaler Kreditzuordnung bleibt das autonome Lernen zweier gemischter Beziehungen unzureichend.

B.5.0 zeigte außerdem:

- Eine zweite vorhandene Beziehung wird unmittelbar selten zerstört.
- Zusätzliche lokale Änderungen können den neuen Abruf beeinträchtigen.
- Positive Gewichtsänderungen führen nicht automatisch zu langfristiger Verbesserung.
- Nach 16 identischen zukünftigen Erfahrungen lag die positiv verstärkte Diagnosegruppe im Mittel 3,63 Prozentpunkte unter der entsprechenden Kontrollgruppe.

Die Ursachen dieser späteren Verschlechterung sind noch nicht ausreichend isoliert.

---

## 2. Zentrale Forschungsfrage

**Unter welchen natürlichen Erfahrungsbedingungen entsteht aus wiederholt positiv bewerteten Handlungen eine autonom abrufbare neuronale Fähigkeit, und wodurch geht diese Fähigkeit bei späteren Erfahrungen wieder verloren?**

Dabei sind drei Prozesse getrennt zu untersuchen:

1. **Erwerb:** Ein zuvor unwirksamer sensorischer Pfad wird durch reale erfolgreiche Erfahrungen verstärkt.
2. **Schwellenübergang:** Die Verbindung erreicht ausreichende neuronale Wirksamkeit für einen autonomen Abruf.
3. **Erhalt oder Verlust:** Weitere Erfahrungen erhalten, verändern oder zerstören die erworbene Fähigkeit.

Eine einzelne erfolgreiche autonome Entscheidung ist noch kein Nachweis für stabile Speicherung.

---

## 3. Forschungshypothesen

### H1 – Fehlende natürliche Wiederholung

Erfolgreiche Explorationsereignisse treten für denselben sensorischen Pfad nicht häufig genug auf, bevor weitere Erfahrungen dessen Gewichte verändern.

### H2 – Unterbrochener Schwellenübergang

Ein bereits verstärkter Sensorpfad erreicht seine autonome Wirksamkeit nicht zuverlässig, weil negative oder konkurrierende Modulation die vorherige Verstärkung wieder abschwächt.

### H3 – Verlust durch konkurrierende positive Erfahrungen

Positive Erfahrungen mit anderen Reizen oder Handlungen verändern gemeinsame neuronale Zustände und können dadurch eine zuvor korrekt gelernte Beziehung beeinträchtigen.

### H4 – Verlust durch negative Erfahrungen

Negative Modulation kann neben einer tatsächlich fehlbewerteten Verbindung weitere lokal berechtigte Synapsen verändern, die für eine andere bereits erlernte Fähigkeit benötigt werden.

### H5 – Instabile inhibitorische Konkurrenz

Die Veränderung inhibitorischer Gewichte kann dazu führen, dass ein ausreichend starker sensorischer Pfad später dennoch keine eindeutige motorische Antwort mehr auslöst.

### H6 – Unzureichende Stabilisierung über weitere Erfahrungen

Eine Verbindung kann die Feuergrenze erreichen, ohne dass ihre funktionale Wirkung unter späteren gemischten Erfahrungsfolgen zuverlässig erhalten bleibt.

Alle Hypothesen sind unabhängig zu prüfen.

Mehrere Ursachen dürfen gleichzeitig zutreffen.

---

## 4. Phase A – Methodische Absicherung

### A1 – Reproduktion

Reproduziere die B.5.0-Referenzen unverändert.

Dazu gehören:

- Normale und inverse Zielzuordnung.
- Drei unabhängige Zufallsstromfamilien.
- No-Learning-Kontrollen.
- Feste und lernbare Hemmung.
- E1- und E2-Auswertung.
- Prospektive erste positive Explorationsereignisse.
- Synaptische Gewichtsänderungen.
- Wiederholungs- und Interferenzdaten.

Die bisherigen 491 Kerntests müssen erhalten bleiben.

### A2 – Korrekte positive Bewertung

Jede Trainingsepisode muss vor der Lernmodulation vollständig ausgewertet werden.

Eine positive Modulation ist nur zulässig, wenn:

1. Eine tatsächliche neuronale Handlung ausgeführt wurde.
2. Genau ein Motorweg als Ergebnis aktiv ist.
3. Die Umwelt diese konkrete Handlung positiv bewertet.

Bei falschen, fehlenden oder doppelten Ausgaben darf keine künstlich positive Rückmeldung erzeugt werden.

Die äußere Umweltbewertung darf weiterhin den Erfolg bestimmen.

Das neuronale System erhält ausschließlich die allgemeine Modulation, keine konkrete richtige Handlungskennung.

### A3 – Wiederholungsexperiment aus B.5.0

Wiederhole den bisherigen diagnostischen Versuch unter ausdrücklicher Prüfung jeder einzelnen Handlung.

Erfasse:

- Präsentierten Reiz.
- Zustand vor Exploration.
- Explorativ aktivierten Weg.
- Tatsächliche vollständige Motorantwort.
- Äußeres Bewertungsergebnis.
- Angewandte Modulation.
- Gewicht vor und nach der Modulation.
- Anschließenden autonomen Abruf.

Vergleiche die Ergebnisse mit den historischen 364/384 beziehungsweise 384/384 Erfolgen.

Falls die bisherigen Zahlen durch die neue Prüfung verändert werden, muss dies ausdrücklich dokumentiert werden.

Die historischen Ergebnisse dürfen nicht überschrieben werden.

---

## 5. Phase B – Natürliche Lernverläufe

Untersuche zunächst das unveränderte neuronale System unter natürlich gemischten Erfahrungsfolgen.

### B1 – Vollständige Gewichtsverläufe

Erfasse für jeden Seed und jede Trainingsepisode:

- Präsentierten Reiz.
- Autonome neuronale Antwort.
- Explorative Aktivität.
- Tatsächliche endgültige Motorantwort.
- Positive, negative oder neutrale Bewertung.
- Relevante Eligibility-Spuren.
- Sämtliche Gewichtsänderungen.
- Sensorische Gewichte beider Reize.
- Inhibitorische Gewichte.
- Aktivität der Entscheidungsneuronen.
- Aktivität der Motorneuronen.

Keine Gewichtsänderung darf lediglich als aggregierter Zähler vorliegen, wenn ihre kausale Zuordnung untersucht werden soll.

### B2 – Natürliche Wiederholung positiver Erfahrungen

Untersuche für jeden sensorischen Pfad:

- Wann tritt die erste erfolgreiche Erfahrung auf?
- Wann tritt die zweite erfolgreiche Erfahrung auf?
- Wie viele andere Erfahrungen liegen dazwischen?
- Wird die gleiche Verbindung erneut verstärkt?
- Wird sie zwischenzeitlich abgeschwächt?
- Wie häufig erreicht sie Gewicht 2?
- Wie häufig entsteht daraus tatsächlich eine autonome richtige Antwort?

Der Begriff „zweite positive Erfahrung“ darf sich nicht allein auf denselben äußeren Reiz beziehen.

Es ist zu prüfen, ob dieselbe relevante neuronale Verbindung tatsächlich erneut eine positive Lernänderung erfährt.

### B3 – Zeitliche Abstände

Erfasse die Abstände zwischen erfolgreichen Erfahrungen.

Unterscheide:

- Direkt aufeinanderfolgende erfolgreiche Erfahrungen.
- Wiederholungen nach wenigen anderen Reizen.
- Wiederholungen nach zahlreichen anderen Erfahrungen.
- Wiederholungen nach zwischenzeitlicher negativer Bewertung.
- Wiederholungen nach bereits erfolgtem autonomen Abruf.

Die Abstände sind zunächst diagnostisch zu untersuchen.

Es darf nicht vorab angenommen werden, dass engere Wiederholungen grundsätzlich besser sind.

---

## 6. Phase C – Der Schwellenübergang

Untersuche gezielt den Übergang eines zuvor nicht autonom wirksamen Sensorpfads zu einer funktionalen Verbindung.

### C1 – Gewichtsübergänge

Erfasse sämtliche natürlichen Übergänge der relevanten sensorischen Verbindungen:

- 0 → 1
- 1 → 2
- 2 → 3
- 3 → 2
- 2 → 1
- 1 → 0

Unterscheide positive und negative Modulation sowie den jeweiligen beteiligten neuronalen Aktivitätszustand.

### C2 – Autonome Wirksamkeit

Prüfe unmittelbar vor und nach jedem relevanten Gewichtsübergang in getrennten diagnostischen Kopien:

- Feuert das zugehörige Entscheidungsneuron?
- Feuert der erwartete Motorweg?
- Feuern konkurrierende Motorwege?
- Bleibt die Ausgabe vollständig aus?
- Welche Rolle spielt inhibitorische Aktivität?
- Welche Membranwerte werden erreicht?

Das Gewicht 2 ist eine im bisherigen Aufbau identifizierte hinreichende Bedingung in bestimmten Zuständen.

Es darf nicht generell angenommen werden, dass Gewicht 2 unabhängig von allen übrigen neuronalen Zuständen stets korrekten autonomen Abruf garantiert.

### C3 – Kausale Einzelgewichtsprüfung

Bei einem neu entstandenen autonomen Erfolg:

1. Erzeuge eine vollständige Kopie vor der entscheidenden Gewichtsänderung.
2. Erzeuge eine vollständige Kopie danach.
3. Nimm die jeweilige Änderung gezielt zurück.
4. Übernimm sie isoliert in den vorherigen Zustand.
5. Vergleiche den autonomen Abruf.

Die übrigen Zustandsunterschiede müssen kontrolliert werden.

Damit ist zu prüfen, ob der Gewichtsschritt im jeweiligen Zustand notwendig und hinreichend war.

---

## 7. Phase D – Stabilität nach dem ersten autonomen Erfolg

### D1 – Erwerbszeitpunkt

Bestimme für jeden Reiz den Zeitpunkt, an dem erstmals eine korrekte eindeutige autonome Antwort entsteht.

Eine einzelne Antwort gilt zunächst nur als erworbene Momentanfähigkeit.

### D2 – Zustandskopien

Erzeuge nach diesem Zeitpunkt vollständige Runtime-Kopien.

Untersuche den Zustand:

- Unmittelbar nach dem ersten autonomen Erfolg.
- Nach einer weiteren Erfahrung.
- Nach 4 weiteren Erfahrungen.
- Nach 8 weiteren Erfahrungen.
- Nach 16 weiteren Erfahrungen.
- Nach 32 weiteren Erfahrungen.
- Nach 64 weiteren Erfahrungen, sofern verfügbar.

Jede diagnostische Evaluation erfolgt ohne weitere Plastizität.

Sie darf den ursprünglichen Trainingslauf nicht beeinflussen.

### D3 – Stabilitätsmessung

Erfasse:

- Bleibt der ursprünglich gelernte Reiz autonom korrekt?
- Wie häufig verschwindet die Antwort?
- Wie häufig entsteht eine falsche oder doppelte Antwort?
- Wird die Fähigkeit später erneut erworben?
- Wie entwickeln sich ihre tragenden Gewichte?
- Bleibt gleichzeitig die zweite Reizbeziehung erhalten?

Unterscheide:

**Erster Erwerb:** Ein zuvor nicht autonom erfolgreicher Pfad wird funktional.

**Stabiler Erwerb:** Die Fähigkeit bleibt über den vorab definierten weiteren Erfahrungszeitraum erhalten.

**Verlust:** Eine zuvor korrekte autonome Antwort ist später nicht mehr korrekt abrufbar.

**Wiedererwerb:** Eine verlorene Fähigkeit wird durch weitere Erfahrung erneut autonom verfügbar.

---

## 8. Phase E – Ursachen eines Fähigkeitsverlusts

Für jeden diagnostizierten Verlust wird die unmittelbar vorangegangene Erfahrungsfolge untersucht.

### E1 – Negativer Lernbeitrag

Prüfe, ob eine negative Modulation die tragende sensorische Verbindung abschwächt.

### E2 – Konkurrierende positive Verstärkung

Prüfe, ob eine positive Erfahrung mit einem anderen neuronalen Weg zu:

- Verstärkung eines konkurrierenden Sensorpfads.
- Doppelaktivierung.
- Veränderter inhibitorischer Konkurrenz.
- Verlust eindeutiger motorischer Ausgabe

führt.

### E3 – Inhibitorische Veränderungen

Untersuche, ob ein veränderter inhibitorischer Beitrag einen zuvor ausreichenden sensorischen Eingang funktional unterdrückt.

### E4 – Ereignis- und Eligibility-Interferenz

Prüfe, welche Eligibility-Kandidaten vor dem Verlust aktiv waren.

Erfasse, ob die zuletzt angewandte Modulation auch Verbindungen verändert hat, deren Beitrag zur bewerteten Handlung nicht kausal nachgewiesen werden kann.

### E5 – Kausale Zustandskopien

Führe mindestens folgende Gegenproben durch:

- Unveränderter Verlauf.
- Rücknahme der zuletzt erfolgten sensorischen Gewichtsänderung.
- Rücknahme der zuletzt erfolgten inhibitorischen Änderung.
- Isolierte Übernahme der konkurrierenden Änderung.
- Neutrale Modulation bei ansonsten identischer Aktivität.
- Deaktivierte Plastizität für die betreffende Episode.
- Unveränderte weitere Erfahrungsfolge nach dem Eingriff.

Ein erfolgreicher Eingriff ist nur dann ein Kausalnachweis, wenn die verglichenen Zustände ansonsten kontrolliert gleich bleiben.

---

## 9. Phase F – Interferenz zwischen zwei Reizbeziehungen

Das übergeordnete Forschungsziel bleibt gleichzeitiges autonomes Lernen zweier Beziehungen.

### F1 – Eine vorhandene Beziehung

Identifiziere Zustände, in denen bereits eine Reiz-Handlungs-Beziehung autonom korrekt ist.

### F2 – Erwerb der zweiten Beziehung

Verfolge die natürlichen erfolgreichen Erfahrungen des zweiten Reizes.

Untersuche insbesondere:

- Wiederholte positive Modulation der neuen sensorischen Verbindung.
- Veränderungen bestehender Verbindungen.
- Beteiligung gemeinsamer Eligibility-Kandidaten.
- Veränderungen inhibitorischer Gewichte.
- Entstehung autonomer Entscheidungen für den zweiten Reiz.

### F3 – Gleichzeitig erfolgreiche Bindungen

Prüfe, wann beide Beziehungen erstmals gleichzeitig autonom korrekt sind.

Untersuche anschließend ihren weiteren Erhalt unter gemischten Erfahrungen.

### F4 – Verlust einer von zwei Beziehungen

Wenn eine zuvor vorhandene Beziehung verloren geht, während die andere gelernt oder weiter verstärkt wird, rekonstruiere die auslösende Ereigniskette.

### F5 – Trennung von unmittelbarer und später Interferenz

Unterscheide:

- Unmittelbaren Verlust durch eine einzelne Gewichtsänderung.
- Verlust nach mehreren gemischten Erfahrungen.
- Allgemeine Schwankungen autonomer Aktivität ohne Gewichtsänderung.
- Verlust durch veränderte inhibitorische Konkurrenz.

Die Ursachen dürfen nicht miteinander vermischt werden.

---

## 10. Phase G – Kontrollierte Erfahrungsfolgen

Ergänzend zu den natürlichen Lernverläufen werden diagnostische Fortsetzungen aus identischen Zustandskopien durchgeführt.

### G1 – Unveränderte natürliche Fortsetzung

Die ursprüngliche Reiz- und Explorationsfolge wird unverändert fortgesetzt.

### G2 – Gleiche Reizfolge, andere Exploration

Die weiteren Reize bleiben identisch, der allgemeine Explorationsstrom wird unabhängig verändert.

### G3 – Gleiche Exploration, andere Reizfolge

Die Explorationsfolge bleibt kontrolliert erhalten, die Reizfolge wird unabhängig verändert.

### G4 – Unterschiedliche Häufigkeit erfolgreicher Erfahrungen

Untersuche dokumentierte, kontrollierte Erfahrungsfolgen mit unterschiedlichen Wiederholungsabständen.

Die Umgebung darf Aufgaben präsentieren und reale Handlungen bewerten.

Sie darf jedoch niemals eine positive Bewertung ohne tatsächlich erfolgreich beobachtete Handlung erzeugen.

### G5 – Keine weitere Plastizität

Derselbe Ausgangszustand wird ohne weitere synaptische Veränderungen fortgesetzt.

### G6 – Kontrollierte diagnostische Gewichtserhaltung

Untersuche in Zustandskopien, ob das Verhindern einzelner identifizierter negativer oder konkurrierender Änderungen eine bereits erworbene Fähigkeit erhalten kann.

Eine solche diagnostische Schutzmaßnahme ist keine zulässige neue Lernregel.

---

## 11. Phase H – Bedingte Forschungsvariante

**B.5.1 setzt keinen neuen neuronalen Mechanismus voraus.**

Erst wenn die Ursachenanalyse einen konkreten lokalen Engpass kausal identifiziert, darf höchstens eine minimale experimentelle Veränderung untersucht werden.

Mögliche Forschungsfragen sind beispielsweise:

- Werden erfolgreiche Erfahrungen ausreichend häufig wiederholt?
- Werden tragende Verbindungen durch fremde Bewertung verändert?
- Kann eine vorhandene lokale Eligibility-Regel den Verlust erklären?
- Liegt eine spezifische Fehlwirkung inhibitorischer Plastizität vor?
- Ist die Gewichtsschrittweite beziehungsweise Feuergrenze tatsächlich unabhängig ursächlich?

### Keine voreiligen Änderungen

Es dürfen nicht ohne vorherigen Kausalnachweis verändert werden:

- Synaptische Lernschrittweite.
- Feuergrenze.
- Allgemeine Eligibility-Regel.
- Länge des Eligibility-Fensters.
- Automatischer Schutz erlernter Gewichte.
- Angeborene Explorationsstrategie.
- Inhibitorische Lernrichtung.

Eine höhere Erfolgsquote durch beliebiges Parameter-Tuning ist kein hinreichender Nachweis einer besseren neuronalen Lernfähigkeit.

### Zulässige minimale Forschungsvariante

Falls ein einzelner lokaler Mechanismus experimentell notwendig erscheint:

1. Dokumentiere den isolierten Fehler.
2. Begründe die vorgeschlagene Änderung anhand der kausalen Diagnose.
3. Implementiere ausschließlich diese Änderung als abschaltbare Forschungsvariante.
4. Vergleiche sie gegen identische Kontrollen.
5. Prüfe ihren eigenständigen kausalen Beitrag.
6. Führe unabhängige Bestätigungsversuche durch.

Es erfolgt keine automatische allgemeine Freigabe.

---

## 12. Lernaufgaben

Sämtliche zentralen Versuchsgruppen durchlaufen:

1. Einzelreiz X→A.
2. Einzelreiz Y→B.
3. Sequenziell X→A, danach Y→B.
4. Sequenziell Y→B, danach X→A.
5. Streng alternierende Erfahrungen.
6. Zufällig gemischte Erfahrungen.
7. Inverse Zuordnung X→B und Y→A.
8. Umkehr der Umweltbewertung ohne technischen Reset.
9. Retention ohne weitere Plastizität.

Zusätzlich sind folgende B.5.1-spezifische Untersuchungen verpflichtend:

- Natürlicher erstmaliger Gewichtsschritt 0 → 1.
- Natürliche erneute positive Verstärkung.
- Natürlicher Schwellenübergang zur autonomen Wirksamkeit.
- Verlust einer bereits autonom korrekten Beziehung.
- Wiedererwerb einer verlorenen Beziehung.
- Gleichzeitige Stabilität beider Beziehungen.

---

## 13. Primäre Messgrößen

### E1 – Autonome neuronale Antwort

E1 wird ausschließlich durch sensorische Reizpräsentation ohne zusätzliche Exploration und ohne weitere Plastizität ermittelt.

Sie bleibt die primäre Erfolgsmetrik.

### E2 – Explorationsgestützte Antwort

E2 wird weiterhin separat aus einer eigenen vollständigen Runtime-Kopie ausgewertet.

### B.5.1-spezifische Messgrößen

Für jeden Sensorpfad:

- Zeitpunkt der ersten positiven Erfahrung.
- Zeitpunkt der zweiten tatsächlich erfolgreichen Erfahrung.
- Anzahl zwischenliegender negativer Erfahrungen.
- Anzahl konkurrierender positiver Erfahrungen.
- Gewichtsentwicklung.
- Zeitpunkt des ersten autonomen Erfolgs.
- Dauer der folgenden autonomen Verfügbarkeit.
- Anzahl und Ursachen späterer Verluste.
- Anzahl und Zeitpunkt erneuter Erwerbsvorgänge.

Für beide Beziehungen gemeinsam:

- Zeitpunkt des erstmaligen gleichzeitigen Erfolgs.
- Dauer des gleichzeitigen Erhalts.
- Häufigkeit einseitiger Verluste.
- Häufigkeit konkurrierender Doppelaktivierung.
- Häufigkeit vollständig fehlender Ausgaben.

Eine isoliert richtige Antwort ohne weiteren Stabilitätsnachweis ist ausdrücklich als Momentaufnahme zu kennzeichnen.

---

## 14. Statistische Versuchsgestaltung

### Entdeckung

Mindestens:

- 64 gepaarte Seeds.
- Drei unabhängig erzeugte Zufallsstromfamilien.
- Normale und inverse Zielzuordnung.
- Identische Ausgangsstrukturen.
- Kontrollierte unabhängige Zufallsquellen.

### Bestätigung

Mindestens weitere 64 unbenutzte Seeds je zentraler Stromfamilie und Zielzuordnung.

Die Bestätigungsdaten dürfen nicht für die Entwicklung einer möglichen neuen Lernregel verwendet werden.

### Trainingsdauer

Die bisherige Referenz mit 128 Trainingsepisoden bleibt erhalten.

Zusätzlich dürfen vorab festgelegte längere Untersuchungen mit beispielsweise 256 und 512 Trainingsepisoden verwendet werden, um natürliche Wiederholung und späteren Fähigkeitsverlust zu untersuchen.

Längere Trainingsdauer darf nicht als Ersatz für den Kausalnachweis verwendet werden.

### Statistische Auswertung

Erforderlich sind:

- Seedbezogene Erfolgsquoten.
- Ereignisbezogene Lernverläufe.
- Verteilungen der Zeiten bis zum ersten autonomen Erwerb.
- Verteilungen der Zeiten bis zum Fähigkeitsverlust.
- Gepaarte Unterschiede.
- 95-%-Bootstrap-Konfidenzintervalle.
- Clusterbewusste Auswertung wiederholter Ereignisse desselben Netzwerks.
- Verwechslungsmatrizen.
- Gewichtstrajektorien.
- Häufigkeit und Abstände positiver Erfahrungen.
- Nachvollziehbare Kausalablationen.

Netze, die während der Beobachtung keine Fähigkeit erwerben oder verlieren, müssen als entsprechend zensierte beziehungsweise nicht eingetretene Ereignisse berücksichtigt werden. Sie dürfen nicht stillschweigend ausgeschlossen werden.

---

## 15. Allgemeine Erfolgskriterien

Das übergeordnete Ziel bleibt zuverlässiges autonomes Mehrfachlernen.

Eine neue Forschungsvariante kann nur dann als allgemein erfolgreich gelten, wenn:

- E1 für X mindestens 75 % erreicht.
- E1 für Y mindestens 75 % erreicht.
- Mindestens 48/64 Seeds beide Schwellen gleichzeitig erfüllen.
- Mindestens 15 Prozentpunkte gepaarter Vorteil gegenüber No-Learning bestehen.
- Das 95-%-Konfidenzintervall vollständig positiv ist.
- Die Ergebnisse mit unabhängigen Seeds reproduziert werden.
- Normale und inverse Zielzuordnung erfolgreich sind.
- Mehrere unabhängige Erfahrungsstromfamilien die Ergebnisse bestätigen.
- Die Fähigkeit ohne weitere Plastizität erhalten bleibt.
- Bereits bestehende Einzelreiz- und Sequenzfähigkeiten nicht wesentlich beeinträchtigt werden.

### Spezifischer Forschungserfolg B.5.1

Unabhängig von diesen allgemeinen Zielwerten gilt B.5.1 als wissenschaftlich erfolgreich abgeschlossen, wenn nachvollziehbar geklärt wurde:

1. Wie häufig natürliche erfolgreiche Wiederholungen die relevante sensorische Verbindung tatsächlich bis zur autonomen Wirksamkeit verstärken.
2. Welche Ereignisse diesen Vorgang unterbrechen.
3. Wann erstmals autonomes Lernen entsteht.
4. Welche Ereignisse eine bestehende Fähigkeit wieder zerstören.
5. Ob diese Verluste durch einzelne lokale Gewichtsänderungen kausal erklärt werden können.
6. Welche Rolle gleichzeitige und spätere konkurrierende Erfahrungen spielen.

Ein negatives Ergebnis hinsichtlich der allgemeinen Lernleistung ist ausdrücklich zulässig.

---

## 16. Architekturgrenzen

Unverändert gelten:

- C++17.
- Vollständig ereignisgesteuerte neuronale Verarbeitung.
- Keine globalen neuronalen Aktualisierungstakte.
- Binäre Feuerereignisse.
- Gewichtete erregende und hemmende Synapsen.
- Lokale G3- und Eligibility-Verarbeitung.
- Drei-Faktor-Plastizität.
- Geschützte angeborene Exploration.
- Keine technisch vorgegebene richtige Handlung.
- Keine semantischen Reizkennungen innerhalb neuronaler Lernzustände.
- Keine globale Motorselektion.
- Keine versteckten Winner-take-all-Regeln.
- Keine automatischen Gewichtsresets.
- Keine dauerhaften Zeitstempel pro Synapse.
- Keine globalen neuronalen Zustandsabfragen zur Handlungsentscheidung.
- Vollständige Persistenz.
- Deterministisch reproduzierbare Ereignisverarbeitung.

Synapsenwachstum bleibt für die primären Experimente deaktiviert.

Diagnostische Zustandskopien dürfen einzelne Gewichte verändern. Solche Eingriffe sind ausdrücklich von zulässigen neuronalen Lernregeln zu trennen.

---

## 17. Technische Validierung

Alle bisherigen 491 Kerntests müssen erhalten bleiben.

Zusätzlich sind Tests erforderlich für:

- Positive Rückmeldung nur nach tatsächlich erfolgreicher Handlung.
- Wiederholte positive Erfahrung bei identischem Reiz.
- Fehlgeschlagene Wiederholung ohne künstlich positives Feedback.
- Natürliche Gewichtsschritte 0 → 1 → 2.
- Abschwächung bereits erworbener sensorischer Gewichte.
- Autonome Antwort vor und nach Schwellenübergang.
- Identische E1-/E2-Anfangszustände.
- Erhalt einer autonomen Fähigkeit.
- Verlust einer autonomen Fähigkeit.
- Wiedererwerb nach Verlust.
- Kausale Zustandskopien.
- Inhibitorische Interferenz.
- Normale und inverse Zuordnung.
- Persistenz vollständiger Zustände.
- Unveränderte Verhaltensregressionen B.4.2 bis B.5.0.

Technische Abbrüche, ungültige Netzwerke und unvollständige Versuche müssen dokumentiert werden und dürfen nicht stillschweigend aus Statistiken verschwinden.

---

## 18. Lieferumfang

Nova-EVE erstellt:

1. Technische Analyse der natürlichen positiven Lernwiederholung.
2. Reproduktion der B.5.0-Referenzen.
3. Korrekte Validierung wiederholter positiver Lernerfahrungen.
4. Vollständige natürliche Gewichtstrajektorien.
5. Statistik der Wiederholung erfolgreicher Erfahrungen.
6. Analyse der Schwellenübergänge.
7. Diagnostische Zustandskopien vor und nach Erwerb.
8. Analyse späterer Fähigkeitsverluste.
9. Kausale Ablationen auslösender Gewichtsänderungen.
10. Untersuchung gleichzeitiger Reizbindungen.
11. Kontrollierte Fortsetzungen mit unterschiedlichen Erfahrungsfolgen.
12. Getrennte E1- und E2-Auswertungen.
13. Unabhängige Bestätigung.
14. Ressourcen- und Persistenzprüfung.
15. Vollständige Rohdaten und statistische Auswertung.
16. Regressionstests.
17. Forschungsbericht B.5.1.
18. Entscheidungsvorlage für B.5.2.

Alle bisherigen Forschungsdaten bleiben unverändert erhalten.

---

## 19. Abschlussentscheidung

Am Ende sind folgende Fragen zu beantworten:

**A.** Wie häufig erreicht ein sensorischer Pfad unter natürlichen Erfahrungen den für autonome Aktivierung notwendigen Wirkungszustand?

**B.** Wie viele tatsächlich erfolgreiche Erfahrungen werden dafür benötigt, und wie häufig werden diese durch andere Ereignisse unterbrochen?

**C.** Welche konkreten neuronalen Zustandsänderungen erzeugen den ersten autonomen Abruf?

**D.** Warum bleibt ein zuvor autonom erfolgreicher Pfad nicht zuverlässig erhalten?

**E.** Sind spätere Verluste überwiegend auf negative Modulation, konkurrierende positive Modulation oder inhibitorische Veränderungen zurückzuführen?

**F.** Welche Rolle spielen andere bereits erworbene Reizbeziehungen?

**G.** Kann eine verlorene Fähigkeit durch weitere natürliche Erfahrung erneut entstehen?

**H.** Ist die vorhandene Plastizität grundsätzlich ausreichend, wenn erfolgreiche Erfahrungen tatsächlich wiederholt werden?

**I.** Ist ein zusätzlicher lokaler neuronaler Mechanismus notwendig, und falls ja, welcher konkrete kausale Befund begründet ihn?

**J.** Welche Konsequenzen ergeben sich für robustes autonomes Mehrfachlernen und Umlernen?

Eine neue allgemeine Lernfunktion darf nur nach isoliertem kausalem Nachweis und unabhängiger Bestätigung freigegeben werden.

Ein negativer Forschungsbefund ist ausdrücklich zulässig.

---

## 20. Forschungsleitlinie

**Eine erfolgreiche Erfahrung kann eine neuronale Verbindung verändern. Mehrere erfolgreiche Erfahrungen können diese Verbindung funktional wirksam machen. Aber erst ihr Erhalt unter weiteren Erfahrungen macht daraus eine zuverlässig verfügbare Fähigkeit.**

EVE darf weder die richtige Handlung noch die zu verstärkende Synapse technisch vorgegeben bekommen.

Sie muss durch eigene Aktivität und reale Umweltbewertung lernen.

Dabei ist eine erworbene Verbindung nicht automatisch wertvoll, nur weil sie einmal erfolgreich war. Weitere Erfahrungen können ihre Bedeutung bestätigen, verändern oder widerlegen.

Eine neue Erfahrung darf bisherige Fähigkeiten verändern. Das ist Bestandteil von Lernfähigkeit.

Aber wir müssen verstehen, wann eine solche Veränderung eine sinnvolle Anpassung ist und wann sie unbeabsichtigte Interferenz erzeugt.

**B.5.1 soll deshalb erstmals den vollständigen Lebenszyklus einer neuronalen Fähigkeit untersuchen: Entstehung, Verstärkung, autonome Nutzung, Erhalt, Verlust und erneuten Erwerb.**
