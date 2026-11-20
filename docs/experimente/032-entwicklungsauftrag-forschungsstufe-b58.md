# Entwicklungsauftrag 032 – Forschungsstufe B.5.8

## Lernen, Nichtlernen und kausale Kreditzuordnung bei fehlender Handlung

**Projekt:** EVE – Emergent Virtual Evolution
**Technologie:** C++17
**Ausgangsbasis:** Konsolidierter Forschungsstand B.5.7 auf `main`
**Forschungsbranch:** `codex/research-stage-b58-learning-stability-and-credit-assignment`
**Auftragstyp:** Kausale Grundlagenforschung mit bedingter, ausdrücklich nicht vorab freigegebener neuronaler Erweiterung

---

## 1. Forschungsgrundsatz

Die folgende Unterscheidung ist eine verbindliche Grundlage dieser Forschungsstufe:

**Lernen können ist eine Fähigkeit. Etwas gelernt haben ist ein Zustand. Beides darf nicht verwechselt werden.**

Ein neuronales System besitzt Lernfähigkeit, wenn es seine Verbindungen aufgrund neuer Erfahrungen funktional verändern kann.

Eine erlernte Fähigkeit liegt vor, wenn seine vorhandene neuronale Struktur unter geeigneten Bedingungen ein bestimmtes Verhalten ermöglicht.

Daraus folgen wesentliche Anforderungen:

- Nicht jede neuronale Aktivität muss eine synaptische Veränderung auslösen.
- Nicht jede Erfahrung muss zu Lernen oder Verlernen führen.
- Eine bereits erlernte Fähigkeit muss nicht durch fortwährende Gewichtsänderungen aufrechterhalten werden.
- Ausbleibende Gewichtsänderungen bedeuten nicht, dass keine Erfahrung verarbeitet wurde.
- Eine Gewichtsänderung allein ist kein Beweis erfolgreichen Lernens.
- Synaptische Stabilität darf nicht mit dem Verlust weiterer Lernfähigkeit verwechselt werden.

**Eine neue Erfahrung darf verarbeitet werden, ohne dass EVE sich dadurch zwangsläufig verändert.**

Gleichzeitig muss EVE bei tatsächlich veränderten Umweltbedingungen weiterhin anpassungsfähig bleiben.

Diese Eigenschaften sollen experimentell untersucht werden.

Es darf keine technische Regel eingeführt werden, die vorhandene Fähigkeiten anhand externer Erfolgskennungen automatisch konserviert.

---

## 2. Ausgangslage

B.5.7 untersuchte den Zusammenhang zwischen lokaler Eligibility, tatsächlicher Motorantwort, Modulation und sensorischen Gewichtsänderungen.

### Gesicherte Ergebnisse

In 48 gezielt ausgewählten nichtrobusten Netzen:

- 9.444 tatsächliche negative sensorische Gewichtsänderungen.
- 7.556 Änderungen (80,01 %) betrafen Synapsen, deren Einzelablation die untersuchte Motorantwort veränderte.
- 1.888 Änderungen (19,99 %) betrafen Synapsen, deren Einzelablation die Motorantwort nicht veränderte.
- 9.376 negative Änderungen (99,28 %) folgten einer fehlenden Motorantwort.
- 68 Änderungen folgten einer falschen eindeutigen Antwort.
- Keine natürliche erfahrungsübergreifende Eligibility.
- Keine natürliche Mehrfachabschwächung derselben Synapse innerhalb eines Modulationskontakts.
- Technische Mehrfachabschwächung ist grundsätzlich möglich.
- Positive sensorische Gewichtsänderungen traten ebenfalls zeitlich gemeinsam mit Fähigkeitsverlusten auf.

Die 80,01 % zeigen eine kausale Wirkung auf die Motorantwort, jedoch nicht automatisch die Notwendigkeit der späteren Gewichtsänderung.

Ebenso bedeutet eine fehlende Wirkung der Einzelablation nicht zwingend, dass die Verbindung irrelevant war. Redundante Beiträge sind möglich.

### Wichtiger Gegenbefund

Das pauschale Unterdrücken negativer Modulation verschlechterte die kurzfristige gemeinsame Abrufbarkeit deutlich.

Nach einer weiteren natürlichen Erfahrung waren:

- 40/48 Netze mit unveränderter negativer Modulation gemeinsam korrekt.
- 4/48 Netze ohne beziehungsweise mit neutraler Modulation gemeinsam korrekt.

Negative Plastizität ist damit nicht grundsätzlich schädlich.

Die verbleibende Frage lautet vielmehr, wann eine Gewichtsänderung funktional gerechtfertigt ist.

### Weitere offene Punkte

- Zwölf B.5.6-Fälle bleiben nicht vollständig erklärt.
- Positive sensorische Konkurrenz ist noch nicht individuell kausal aufgelöst.
- S4-d16 ist stark abhängig von zeitlichen Erfahrungsabständen.
- Bisher fehlt eine systematische Untersuchung erlernter Fähigkeiten bei weiterhin aktiver, aber nicht fortwährend wirksamer Plastizität.

---

## 3. Zentrale Forschungsfragen

B.5.8 untersucht drei zusammenhängende Fragen.

### Frage A – Muss eine Erfahrung eine Gewichtsänderung auslösen?

Kann EVE vorhandene Fähigkeiten anwenden und Erfahrungen verarbeiten, ohne dabei fortwährend neue synaptische Veränderungen vorzunehmen?

### Frage B – Wann ist eine Gewichtsänderung notwendig?

Unter welchen lokalen Bedingungen führt eine positive oder negative Veränderung zu einer nachweislichen Verbesserung zukünftiger Fähigkeiten?

### Frage C – Was bedeutet eine fehlende Handlung?

Ist eine ausbleibende Motorantwort ein hinreichender Anlass für negative synaptische Modulation, oder muss zwischen unterschiedlichen Ursachen und Folgen unterschieden werden?

**Die Forschung soll zunächst diese Fragen beantworten, nicht eine neue Lernregel implementieren.**

---

## 4. Definitionen und Messkriterien

### 4.1 Neuronale Aktivität

Ein oder mehrere Neuronen empfangen Eingänge, verändern ihre Membranpotenziale oder feuern.

Neuronale Aktivität ist nicht gleichbedeutend mit Lernen.

### 4.2 Erfahrung

Eine zeitlich zuordenbare Interaktion zwischen sensorischem Eingang, neuronaler Verarbeitung, gegebenenfalls ausgeführter Handlung und äußerer Rückmeldung.

Auch eine ausbleibende Handlung kann Bestandteil einer Erfahrung sein.

### 4.3 Synaptische Veränderung

Eine tatsächlich eingetretene Veränderung eines Synapsengewichts.

Zu unterscheiden sind:

- Positive Veränderung.
- Negative Veränderung.
- Keine Veränderung.

### 4.4 Erlernte Fähigkeit

Eine auf vorhandenen neuronalen Zuständen und Verbindungen beruhende, reproduzierbar abrufbare Verhaltensfähigkeit.

Sie wird durch geeignete autonome Prüfungen nachgewiesen, nicht allein durch Gewichtswerte.

### 4.5 Lernfähigkeit

Die Möglichkeit, aufgrund neuer Erfahrungen das zukünftige Verhalten funktional anzupassen.

Sie muss durch kontrollierte neue Erfahrungen und spätere Verhaltensprüfungen untersucht werden.

### 4.6 Stabilität

Eine Fähigkeit bleibt über zusätzliche Erfahrungen und zeitliche Veränderungen verfügbar.

Stabilität darf nicht allein anhand unveränderter Gewichte festgestellt werden.

### 4.7 Plastizität ohne funktionalen Gewinn

Eine synaptische Veränderung tritt ein, ohne dass sich in den festgelegten Folgeprüfungen ein positiver funktionaler Beitrag nachweisen lässt.

Diese Kategorie bedeutet nicht automatisch, dass die Änderung langfristig nutzlos ist.

### 4.8 Keine Veränderung trotz Erfahrung

Ein neuronales System verarbeitet eine Erfahrung, ohne dabei synaptische Gewichte zu verändern.

Dieser Zustand ist als eigenständiges Forschungsresultat zu betrachten, nicht als Fehler oder fehlgeschlagenes Lernen.

---

## 5. Forschungshypothesen

### H1 – Keine notwendige Dauerplastizität

Bereits erlernte Fähigkeiten können über weitere natürliche Erfahrungen erhalten bleiben, ohne dass dafür ständig synaptische Gewichte verändert werden müssen.

### H2 – Weiterhin vorhandene Lernfähigkeit

Ein Netz kann während stabiler Phasen geringe oder keine Gewichtsänderungen zeigen und bei später veränderten Umweltbedingungen dennoch neue Beziehungen erwerben.

### H3 – Unnötige Plastizität

Ein Teil der beobachteten Gewichtsänderungen ist für den aktuellen oder nachfolgenden funktionalen Erfolg nicht erforderlich.

### H4 – Notwendige negative Plastizität

Bestimmte negative Gewichtsänderungen sind für das Korrigieren erfolgloser Verhaltensweisen und für späteres Umlernen notwendig.

### H5 – Unterschiedliche Ursachen fehlender Handlungen

Eine fehlende Motorantwort kann verschiedene lokale Ursachen haben, die nicht zwangsläufig dieselbe synaptische Änderung rechtfertigen.

### H6 – Unpräzise Kreditzuordnung

Eine gültige Eligibility identifiziert zeitlich berechtigte synaptische Kandidaten, nicht zwingend die tatsächlich verantwortlichen Verbindungen.

### H7 – Positive Konkurrenz

Positive Gewichtsänderungen können durch veränderte neuronale Konkurrenz ebenfalls zuvor erworbene Beziehungen beeinträchtigen.

### H8 – Neutrale und fehlende Modulation

Ein neutrales Modulationssignal und vollständig fehlendes Feedback können trotz gleicher unmittelbarer Gewichtsänderung unterschiedliche spätere Auswirkungen haben.

### H9 – Informationsgrenze

Die vorhandenen lokalen neuronalen Zustände könnten nicht ausreichen, um alle notwendigen und unnötigen synaptischen Änderungen eindeutig zu unterscheiden.

### H10 – Zeitliche Abhängigkeit

Die Stabilität bereits erlernter Fähigkeiten und die Notwendigkeit weiterer Plastizität hängen von der zeitlichen Abfolge der Erfahrungen ab.

---

## 6. Phase A – Referenz und technisches Audit

### A1 – Ausgangsbasis

Erstelle den Forschungsbranch vom konsolidierten B.5.7-Stand auf `main`.

Prüfe vor Änderungen:

- Aktuellen Git-Stand.
- Vollständige Forschungsdokumentation.
- Vorhandene Regressionstests.
- Referenzdaten.
- Persistenzformat.
- Tatsächliche Standardkonfiguration.

### A2 – Referenzvarianten

Verwende:

- B.5.7-S4-d16 als experimentelle Hauptreferenz.
- R2-50 ohne S4.
- S5 mit deaktivierter S4.
- No-Learning.
- Normale und inverse Zielzuordnung.
- Drei unabhängige Zufallsstromfamilien.

S4 bleibt im Produktionsstandard deaktiviert.

### A3 – Bestehende Plastizitätswege

Untersuche ausdrücklich:

1. Aktivitätsabhängige synaptische Verstärkung.
2. Eligibility-basierte Drei-Faktor-Plastizität.
3. Positive Modulation.
4. Negative Modulation.
5. Neutrale Modulation.
6. Ausbleibende Modulation.

Die Forschungsreferenz verwendet für den ersten Mechanismus `learning_enabled = false`, während die Drei-Faktor-Plastizität aktiv bleibt.

Dieser Unterschied muss bei sämtlichen Kontrollen berücksichtigt werden.

### A4 – Reproduktion

Reproduziere die zentralen B.5.7-Ergebnisse.

Historische Daten dürfen nicht verändert werden.

---

## 7. Phase B – Tatsächliche Lernereignisse erfassen

### B1 – Vollständige Ereigniskette

Erfasse für jede Erfahrung:

- Sensorischer Eingang.
- Neuronale Feuerereignisse.
- Tatsächliche Motorantwort.
- Exploration.
- Tatsächlich ausgeführte Handlung.
- Äußere Bewertung.
- Modulationspolarität.
- Erreichte lokale Modulationskontakte.
- Eligibility-Einträge.
- Positive Gewichtsänderungen.
- Negative Gewichtsänderungen.
- Ausbleibende Gewichtsänderungen.
- Autonome Abrufbarkeit beider Beziehungen vor und nach der Erfahrung.

### B2 – Keine Veränderung

Unterscheide die Ursachen tatsächlich unveränderter Gewichte:

- Keine Modulation.
- Neutrale Modulation.
- Keine aktive Eligibility.
- Keine erreichte lokale Modulationsroute.
- Geschützte Synapse.
- Erreichte Gewichtsgrenze.
- Andere technische Begrenzung.
- Vorhandene Aktivität ohne wirksames Plastizitätsereignis.

### B3 – Funktionale Wirkung

Prüfe nach jeder relevanten Gewichtsänderung oder Nullveränderung:

- Unmittelbare nächste Antwort.
- Andere Reizbeziehung.
- Wiederholung derselben Erfahrung.
- Verhalten nach zusätzlichen natürlichen Erfahrungen.

### B4 – Keine Ergebnisunterstellung

Eine Veränderung wird nicht allein deshalb als sinnvoll bezeichnet, weil sie stattgefunden hat.

Eine Nullveränderung wird nicht allein deshalb als sinnvoll bezeichnet, weil die Gewichte erhalten geblieben sind.

Die Einordnung erfolgt anhand kontrollierter Gegenproben.

---

## 8. Phase C – Fehlende Motorantworten kausal untersuchen

### C1 – Tatsächliche Handlungsklassen

Unterscheide:

| Klasse | Ergebnis |
|---|---|
| M0 | Korrekte eindeutige autonome Handlung |
| M1 | Falsche eindeutige autonome Handlung |
| M2 | Keine autonome Antwort, aber erfolgreiche Exploration |
| M3 | Keine autonome Antwort und keine erfolgreiche explorative Handlung |
| M4 | Doppelantwort |
| M5 | Motorische Aktivität ohne gültigen Handlungsabschluss |
| M6 | Technischer Abbruch oder unvollständige Verarbeitung |

Die Kategorien dienen ausschließlich der Forschungsauswertung.

### C2 – Lokale Ursachen

Untersuche bei Nichtantworten:

- Sensorische Eingangsstärke.
- Inhibitorische Wirkung.
- Refraktärzustände.
- Membranlage.
- Recovery.
- S4-Relaxation.
- Erregungsweiterleitung.
- Motorische Feuergrenze.
- Beteiligung der geschützten Exploration.

### C3 – Kontrollierte Zustandskopien

Prüfe, welche einzelnen oder kombinierten Faktoren die fehlende Motorantwort verursachen.

### C4 – Lernfrage

Untersuche anschließend, ob eine negative sensorische Gewichtsänderung geeignet ist, die nachgewiesene Ursache zu korrigieren.

Eine Nichtantwort allein darf noch nicht als hinreichender Kausalnachweis für eine bestimmte synaptische Abschwächung gelten.

---

## 9. Phase D – Negatives, neutrales und fehlendes Feedback

### D1 – Identischer Ausgangszustand

Erzeuge vollständige Runtime-Kopien unmittelbar vor einer tatsächlich beobachteten Modulation.

### D2 – Feedbackvarianten

**F0 – Natürlich**

Unveränderte äußere Bewertung und Modulation.

**F1 – Kein Feedback**

Es wird kein Modulationssignal erzeugt.

**F2 – Neutrales Feedback**

Ein neutrales Signal wird über die vorhandene neuronale Modulationsroute zugestellt.

**F3 – Diagnostische lokale Kandidatenbegrenzung**

Nur eine vorab festgelegte Teilmenge berechtigter Kandidaten wird verändert.

**F4 – Diagnostische Verzögerung**

Die Rückmeldung erfolgt zu einem anderen, vorher festgelegten Zeitpunkt.

**F5 – Keine wirksame Gewichtsänderung**

Der tatsächliche Feedback- und Eligibility-Ablauf bleibt für die Diagnose nachvollziehbar, während synaptische Schreiboperationen in einer getrennten Forschungskopie unterdrückt werden.

### D3 – Unterschied F1, F2 und F5

Diese Varianten dürfen nicht gleichgesetzt werden.

Zu prüfen sind:

- Anzahl und Art der Modulationsereignisse.
- Eligibility-Verbrauch.
- Eligibility-Fortbestand.
- Ablauf vorhandener Spuren.
- Ereignisreihenfolge.
- Synaptische Gewichte.
- Spätere Lernmöglichkeiten.

### D4 – Kontrollierte Fortsetzung

Alle Kopien erhalten anschließend gepaarte, natürliche Erfahrungsfolgen.

### D5 – Keine pauschale Schlussfolgerung

Falls neutrale Modulation kurzfristig schadet, folgt daraus nicht, dass sämtliche Nullveränderungen schädlich sind.

Falls neutrale Modulation kurzfristig hilft, folgt daraus nicht, dass negatives Feedback grundsätzlich unnötig ist.

---

## 10. Phase E – Erlernte Fähigkeiten ohne fortwährende Plastizität

**Diese Phase ist ein zentraler Bestandteil von B.5.8.**

### E1 – Ausgangszustände

Wähle neuronale Netze, in denen beide Beziehungen nachweislich autonom verfügbar sind.

Verwende getrennte Populationen:

- Deutlich robuste Netze.
- Knapp robuste Netze.
- Zeitweise gemeinsam korrekte, aber nichtrobuste Netze.

### E2 – Kontrollgruppen

Erzeuge identische vollständige neuronale Zustandskopien.

**P0 – Normale Plastizität**

Weitere natürliche Erfahrungen bei unveränderten Umweltbedingungen.

**P1 – Gewichtsänderungen experimentell gesperrt**

Dieselben Eingangs- und Explorationsströme, jedoch ohne synaptische Gewichtsschreiboperationen.

Die neuronale Aktivität und die vorhandenen Modulationswege sollen soweit technisch möglich unverändert ausführbar bleiben.

**P2 – Normale Plastizität mit späterer Umweltveränderung**

Zunächst bekannte Erfahrungen, anschließend Änderung der äußeren Bewertungsbedingungen.

**P3 – Gesperrte Plastizität mit späterer Umweltveränderung**

Vergleich zu P2, um scheinbare Anpassung durch transiente Zustände von tatsächlichem synaptischem Umlernen zu unterscheiden.

### E3 – Technische Anforderungen an P1 und P3

Eine experimentelle Plastizitätssperre darf nicht unbeabsichtigt:

- Neuronale Eingänge entfernen.
- Motorische Feuerereignisse unterdrücken.
- Exploration deaktivieren.
- Eligibility-Spuren löschen.
- Modulationssignale entfernen.
- Membranzustände zurücksetzen.

Sie soll ausschließlich die tatsächlichen synaptischen Gewichtsschreiboperationen kontrollieren.

Falls dies mit der vorhandenen Konfiguration nicht isoliert möglich ist, darf eine ausdrücklich experimentelle, standardmäßig deaktivierte Diagnoseschnittstelle implementiert werden.

Diese darf keine Produktionsfunktion werden.

### E4 – Stabile Phase

Untersuche über:

- 16 Erfahrungen.
- 32 Erfahrungen.
- 64 Erfahrungen.
- 128 Erfahrungen.
- 256 Erfahrungen.
- 512 Erfahrungen.

Dabei erfassen:

- Anzahl positiver Gewichtsänderungen.
- Anzahl negativer Gewichtsänderungen.
- Anzahl unveränderter Erfahrungen.
- E1 X und Y.
- E2 X und Y.
- Gemeinsame Verfügbarkeit.
- Fähigkeitsverluste.
- Wiedererwerb.
- Membran- und Eligibility-Dynamik.

### E5 – Forschungsfrage

Kann die bereits erworbene neuronale Struktur die geforderte Fähigkeit über weitere Erfahrungen bereitstellen, ohne zusätzliche synaptische Veränderungen zu benötigen?

### E6 – Notwendigkeit weiterer Plastizität

Vergleiche P0 und P1.

Falls beide gleichwertig bleiben, ist für die untersuchte stabile Phase keine weitere Gewichtsänderung als notwendig nachgewiesen.

Falls P0 deutlich besser wird, muss ermittelt werden, welche Änderungen den Vorteil bewirken.

### E7 – Keine automatische Stabilitätsregel

Ein positives Ergebnis rechtfertigt keine technische Sperre erfolgreicher Synapsen.

Es ist ein Forschungsbefund über die Beziehung zwischen bestehender Struktur und weiterem Lernen.

---

## 11. Phase F – Lernfähigkeit nach stabilen Phasen

### F1 – Ausgangspunkt

Verwende dieselben Netze nach einer längeren Phase unveränderter Umweltbedingungen.

### F2 – Umweltveränderung

Ändere die äußere Bewertung, ohne das neuronale Netzwerk zurückzusetzen.

### F3 – Wieder aktivierbare Plastizität

Prüfe getrennt:

- Durchgehend aktive Plastizität.
- Während einer stabilen Diagnosephase unterdrückte und anschließend wieder aktivierte Gewichtsänderungen.
- Weiterhin deaktivierte Plastizität.

### F4 – Tatsächliches Umlernen

Erfasse:

- Zeitpunkt erster neuer positiver Erfahrung.
- Erste wirksame neue synaptische Änderung.
- Erste neue autonome Präferenz.
- Stabilität der geänderten Beziehung.
- Erhaltene Exploration.

### F5 – Trennung der Fähigkeiten

Ein Netz, das während einer stabilen Phase keine Gewichtsänderung benötigt, soll bei neuer Umweltbewertung weiterhin lernfähig sein.

### F6 – Interpretation

Der Erhalt bestehender Fähigkeiten und die Anpassung an neue Bedingungen sind getrennte Erfolgskriterien.

Ein unverändertes Netz kann eine vorhandene Fähigkeit besitzen und gleichzeitig ohne aktive Plastizität nicht mehr umlernen.

Das ist kein Widerspruch, sondern genau der zu untersuchende Unterschied.

---

## 12. Phase G – Notwendige und unnötige synaptische Veränderungen

### G1 – Negative Änderungen

Untersuche einzelne negative Gewichtsänderungen durch kontrollierte Zustandskopien.

### G2 – Positive Änderungen

Untersuche analog positive Veränderungen.

### G3 – Keine Änderung

Vergleiche jeweils mit einer Kopie, in der die konkrete Änderung ausbleibt.

### G4 – Funktionale Folgen

Mindestens:

- Nächste autonome Antwort.
- Gegenbeziehung.
- Retention.
- Weitere natürliche Erfahrungen.
- Umweltumkehr.

### G5 – Mehrere betroffene Verbindungen

Untersuche kombinierte Kandidaten, wenn einzelne Ablationen wegen Redundanz keine ausreichende Aussage erlauben.

### G6 – Zeitlich neutrale Veränderungen

Erfasse synaptische Änderungen, die kurzfristig weder positive noch negative funktionale Effekte erkennen lassen.

Diese bleiben als eigene Klasse dokumentiert.

Sie dürfen nicht vorschnell als unnötig bezeichnet werden.

### G7 – Langfristiger Nutzen

Eine Gewichtsänderung kann erst durch spätere Erfahrungen wirksam werden.

Daher muss die Bewertung unterschiedliche Folgezeiträume berücksichtigen.

---

## 13. Phase H – Positive sensorische Konkurrenz

B.5.7 dokumentierte positive Gewichtsänderungen, die zeitlich mit Verlusten anderer Beziehungen zusammenfielen.

### H1 – Einzelne positive Änderungen

Bestimme deren unmittelbare kausale Wirkung.

### H2 – Kombinierte positive Änderungen

Untersuche, ob mehrere gleichzeitig verstärkte Synapsen gemeinsam einen unerwünschten motorischen Effekt erzeugen.

### H3 – Inhibitorische Konkurrenz

Prüfe die Wechselwirkung positiver sensorischer Verstärkung mit vorhandenen inhibitorischen Beiträgen.

### H4 – Gegenbeziehung

Eine Verbesserung von X darf nicht automatisch als Gesamterfolg gelten, wenn Y dadurch beeinträchtigt wird.

### H5 – Forschungsgrenze

Es darf keine pauschale Unterdrückung positiver Verstärkung eingeführt werden.

---

## 14. Phase I – Die zwölf offenen historischen Fälle

Die zwölf in B.5.6 nicht vollständig erklärten Fälle bleiben ausdrücklich Teil der Untersuchung.

### I1 – Rekonstruktion

Verfolge für jeden Fall:

- Letzten gemeinsamen erfolgreichen Zustand.
- Nachfolgende Erfahrungen.
- Positive und negative synaptische Änderungen.
- Modulationskontakte.
- Eligibility.
- Motorische Nichtantworten.
- Explorative Handlungen.
- Membranzustände.
- Recovery und S4.

### I2 – Kombinierte Gegenproben

Untersuche sensorische, inhibitorische und transiente Faktoren einschließlich ihrer Wechselwirkungen.

### I3 – Nullveränderung

Prüfe, ob das Ausbleiben ausgewählter tatsächlicher Gewichtsänderungen den Fehler verhindert hätte.

### I4 – Wissenschaftliche Grenze

Nicht erklärbare Fälle bleiben offen.

Keine nachträgliche Einordnung allein zur Vervollständigung der Statistik.

---

## 15. Phase J – Lokal verfügbare Informationen

**Die externe Forschungsdiagnose darf mehr wissen als die einzelne Synapse.**

Dies ist unbedingt einzuhalten.

### J1 – Lokales Informationsinventar

Erfasse:

- Eingangssignal.
- Synapsengewicht.
- Erregender oder hemmender Effekt.
- Membranpotenzial.
- Neuronales Feuern.
- Lokale Aktivität.
- G3.
- Eligibility.
- Modulationssignal.
- Zeitliche Ereignisbeziehungen.

### J2 – Nicht lokal verfügbare Informationen

Davon zu trennen sind:

- Globale richtige Handlung.
- Technische Bezeichnung X oder Y.
- Frühere Erfolgsstatistik des gesamten Netzes.
- Aktueller Erfolg der Gegenbeziehung.
- Extern diagnostizierte kausale Notwendigkeit.

### J3 – Unterscheidbarkeit

Prüfe, ob notwendige und unnötige Gewichtsänderungen anhand lokal vorhandener Informationen unterschiedlich charakterisiert werden können.

### J4 – Gleiche lokale Zustände

Untersuche ausdrücklich Situationen, in denen lokal identische Zustände zu unterschiedlichen globalen Bewertungen gehören.

Falls solche Fälle existieren, ist eine sichere ausschließlich lokale Unterscheidung möglicherweise nicht möglich.

### J5 – Keine erfundene Information

Eine neue Lernregel darf keine Informationen verwenden, die ihr neuronaler Wirkpfad nicht tatsächlich liefert.

---

## 16. Phase K – Unterschiedliche Zeitbedingungen

B.5.6 und B.5.7 zeigten eine erhebliche Abhängigkeit vom Episodenraster.

Daher sind mindestens zu untersuchen:

- Fester Abstand 12.
- Fester Abstand 16.
- Fester Abstand 32.
- Unregelmäßige Abstände.
- Kurze wiederholte Reizfolgen.

### K1 – Zeitliche Wirkpfade

Erfasse:

- Sensorische Aktivität.
- Feuerereignisse.
- Eligibility.
- Motorhandlung.
- Exploration.
- Modulation.
- Recovery.
- Subthreshold-Relaxation.

### K2 – Keine Rasteroptimierung

S4-d16 bleibt unverändert.

Die Versuche dienen der Ursachenprüfung, nicht der nachträglichen Auswahl einer günstigeren Zeitkonstante.

### K3 – Kontinuierliche Verarbeitung

Unterscheide unregelmäßig zeitlich angeordnete, weiterhin getrennt bewertete Erfahrungen von tatsächlich überlappenden Ereignisströmen.

Falls eine echte Überlappung nicht untersucht wird, muss dies ausdrücklich dokumentiert werden.

---

## 17. Phase L – Bedingte neue lokale Lernregel

**B.5.8 gibt vorab keine neue neuronale Lernfunktion frei.**

Eine minimale experimentelle Erweiterung ist erst zulässig, wenn:

1. Eine konkrete schädliche Änderungsklasse kausal nachgewiesen wurde.
2. Eine geeignete lokale Unterscheidungsinformation vorhanden ist.
3. Eine isolierte Intervention die Ursache tatsächlich beeinflusst.
4. Notwendige negative Korrekturen erhalten bleiben.
5. Positive Konkurrenz berücksichtigt ist.
6. Die Gegenbeziehung nicht systematisch beeinträchtigt wird.
7. Die Fähigkeit zum Umlernen erhalten bleibt.
8. Der Mechanismus deaktivierbar und persistent ist.
9. Die unabhängige Bestätigung positiv ausfällt.

### Mögliche Forschungsfrage

Kann ein lokaler Modulationskontakt anhand vorhandener neuronaler Informationen wirksam bleiben, ohne zwangsläufig eine Gewichtsänderung auszuführen?

Dies ist eine offene Frage, keine Implementierungsanweisung.

### Nicht zulässig

- Automatischer Gewichtsschutz nach erfolgreichem Erwerb.
- Globale Entscheidung, dass EVE ausreichend gelernt habe.
- Wiederherstellung früher erfolgreicher Gewichte.
- Verwendung externer Zielkennungen.
- Technische Auswahl der richtigen Motorantwort.
- Pauschale Abschaltung negativer Plastizität.
- Pauschale Verstärkung nach Nichtantwort.
- Erzwungene positive Bewertung ohne erfolgreiche Handlung.
- Technischer Winner-take-all-Mechanismus.
- Globale neuronale Zustandsverwaltung.
- Optimierung der S4-Lebensdauer.

---

## 18. Kontrollgruppen

| Gruppe | Inhalt |
|---|---|
| K0 | Unveränderte B.5.7-Referenz |
| K1 | Natürliches positives und negatives Lernen |
| K2 | Kein Feedback |
| K3 | Neutrales Feedback |
| K4 | Diagnostisch keine Gewichtsänderung |
| K5 | Normale Plastizität bei stabiler Umwelt |
| K6 | Gesperrte Gewichtsänderungen bei stabiler Umwelt |
| K7 | Aktive Plastizität bei Umweltumkehr |
| K8 | Gesperrte Plastizität bei Umweltumkehr |
| K9 | Reaktivierung nach diagnostischer Plastizitätssperre |
| K10 | Kausale Nichtantwort-Analyse |
| K11 | Einzelne negative Änderung |
| K12 | Kombinierte negative Änderungen |
| K13 | Positive sensorische Konkurrenz |
| K14 | Redundante synaptische Beiträge |
| K15 | Zwölf offene historische Fälle |
| K16 | Unterschiedliche Erfahrungsabstände |
| K17 | No-Learning |
| K18 | Gegebenenfalls neue lokale Forschungsregel |
| K19 | Identische Regel deaktiviert |

K18 und K19 sind ausschließlich bei positivem Kausalnachweis umzusetzen.

---

## 19. Versuchsumfang und Statistik

### Entdeckung

Mindestens:

- 64 Seeds je Stromfamilie und Zuordnung.
- Drei unabhängige Zufallsstromfamilien.
- Normale und inverse Zuordnung.
- 512 natürliche Trainingsepisoden je zentralem Netz.

### Stabile Lernphasen

Zusätzliche Erfahrungen nach bereits nachgewiesenem Erwerb:

- 16
- 32
- 64
- 128
- 256
- 512

### Bestätigung

Mindestens weitere 64 bisher unbenutzte Seeds je zentraler Kombination.

Bestehende B.5.7-Bestätigungsdaten dürfen nicht nochmals als unabhängige Bestätigung einer daraus entwickelten Regel ausgegeben werden.

### Statistische Auswertung

Erforderlich sind:

- Gepaarte Unterschiede.
- 95-%-Bootstrap-Konfidenzintervalle.
- Seedbasierte Clusterung.
- E1 X und Y.
- E2 X und Y.
- Gemeinsame Robustheit.
- Retention.
- Zeit bis zum Wiedererwerb.
- Zeit bis zur neuen Fähigkeit.
- Positive, negative und Null-Gewichtsänderungen.
- Funktionale Wirkung der Änderungen.
- Stabilität ohne weitere Plastizität.
- Anpassungsfähigkeit nach Umweltveränderung.

Mehrere Erfahrungen desselben Netzes dürfen nicht als unabhängige Seeds gewertet werden.

---

## 20. Erfolgskriterien

### Wissenschaftlicher Erfolg

B.5.8 gilt als erfolgreich, wenn:

1. Neuronale Aktivität, Erfahrung, Feedback und tatsächliche synaptische Veränderung getrennt ausgewertet wurden.
2. Die Ursachen fehlender Motorantworten nachvollziehbar untersucht wurden.
3. Negatives, neutrales und fehlendes Feedback kausal verglichen wurden.
4. Null-Gewichtsänderungen als eigenständige Ergebnisse berücksichtigt wurden.
5. Bereits erlernte Fähigkeiten unter aktiver und gesperrter Plastizität verglichen wurden.
6. Der Unterschied zwischen Fähigkeitserhalt und erneuter Lernfähigkeit nachgewiesen oder klar eingegrenzt wurde.
7. Notwendige negative Korrekturen erhalten bleiben.
8. Positive sensorische Konkurrenz untersucht wurde.
9. Die zwölf offenen Fälle erneut geprüft wurden.
10. Die Grenzen lokal verfügbarer Information dokumentiert wurden.
11. Unabhängige Bestätigungsversuche vorliegen.

Eine neue Lernregel ist für den wissenschaftlichen Abschluss nicht erforderlich.

### Allgemeine Freigabe einer neuen Funktion

Dafür gelten weiterhin:

- Mindestens 75 % E1 für X.
- Mindestens 75 % E1 für Y.
- Mindestens 48/64 gleichzeitig robuste Seeds je Stromfamilie und Zuordnung.
- Mindestens 15 Prozentpunkte gepaarter Lernvorteil gegenüber No-Learning.
- Vollständig positives 95-%-Konfidenzintervall.
- Eigener kausaler Beitrag gegenüber S4-d16.
- Unabhängige Bestätigung.
- Retention.
- Erhaltenes Umlernen.
- Geschützte Exploration.
- Determinismus.
- Vollständige Persistenz.
- Begrenzter Ressourcenbedarf.

---

## 21. Technische Validierung

Alle bisherigen **637 Kerntests** müssen bestehen bleiben.

Neue Tests sind vorzusehen für:

- Erfahrung ohne Gewichtsänderung.
- Neuronale Aktivität ohne Lernen.
- Fehlendes Feedback.
- Neutrales Feedback.
- Negative Modulation.
- Positive Modulation.
- Eligibility-Fortbestand.
- Eligibility-Verbrauch.
- Fehlende autonome Motorantwort.
- Erfolgreiche Exploration.
- Erfolgslose Exploration.
- Doppelantwort.
- Plastizitätssperre ohne Unterdrückung neuronaler Aktivität.
- Stabile Fähigkeit mit aktivierter Plastizität.
- Stabile Fähigkeit mit gesperrten Gewichtsänderungen.
- Umweltumkehr mit aktiver Plastizität.
- Umweltumkehr mit gesperrter Plastizität.
- Reaktivierung der Plastizität nach einer Sperrphase.
- Positive sensorische Konkurrenz.
- Redundante synaptische Beiträge.
- Normale und inverse Zielzuordnung.
- Unterschiedliche Zeitabstände.
- Geschützte Exploration.
- Unveränderte S4-Abschaltkontrolle.
- Persistenz aktiver Eligibility.
- Persistenz ausstehender Modulation.
- Deterministische Fortsetzung.
- Gültige Netzwerkzustände.

Keine historischen Tests entfernen oder abschwächen.

---

## 22. Persistenz und Ressourcen

Neue Diagnoseschnittstellen dürfen keine unkontrollierten Produktionszustände erzeugen.

Erfasse:

- Speicherbedarf pro Neuron.
- Speicherbedarf pro Synapse.
- Anzahl zusätzlicher Ereignisse.
- Maximale Queuegröße.
- CPU-Zeit.
- Anzahl diagnostischer Kopien.
- Datenumfang.
- Persistenzgültigkeit.
- Exakte Fingerprints.

Die experimentelle Unterdrückung von Gewichtsschreiboperationen muss sich in vollständigen Zustandskopien reproduzieren lassen.

Historische Forschungsdaten bleiben unverändert.

Neue große Rohdaten sind komprimiert abzulegen.

---

## 23. Lieferumfang

Nova-EVE erstellt:

1. Vollständige Reproduktion der B.5.7-Referenz.
2. Technisches Audit der bestehenden Plastizitätswege.
3. Klassifikation tatsächlicher Gewichtsänderungen.
4. Untersuchung fehlender Motorantworten.
5. Vergleich negativer, neutraler und ausbleibender Modulation.
6. Messung und Klassifikation von Null-Gewichtsänderungen.
7. Experimentelle Kontrolle synaptischer Gewichtsschreiboperationen.
8. Untersuchung stabiler Fähigkeiten ohne weitere Gewichtsänderungen.
9. Untersuchung fortbestehender Lernfähigkeit nach stabilen Phasen.
10. Umweltumkehr mit aktiver und gesperrter Plastizität.
11. Kausale Einzel- und Kombinationsablationen.
12. Analyse notwendiger negativer Korrekturen.
13. Kausale Untersuchung positiver sensorischer Konkurrenz.
14. Untersuchung redundanter Beiträge.
15. Neue Analyse der zwölf historischen offenen Fälle.
16. Bestandsaufnahme lokal verfügbarer Informationen.
17. Unterschiedliche zeitliche Erfahrungsfolgen.
18. Unabhängige Bestätigung.
19. Gegebenenfalls minimale lokale Forschungsvariante.
20. Vollständige Abschaltkontrolle.
21. Regressionstests.
22. Persistenz- und Ressourcenprüfungen.
23. Forschungsbericht B.5.8.
24. Technische Kausalanalyse.
25. Entscheidungsvorlage B.5.9.
26. Explizite Merge-Empfehlung.

---

## 24. Abschlussfragen

**A.** Muss eine Erfahrung zwangsläufig eine synaptische Veränderung auslösen?

**B.** Können bereits erworbene Fähigkeiten ohne fortwährende Gewichtsänderungen stabil verfügbar bleiben?

**C.** Bleibt ein solches Netzwerk bei später veränderter Umwelt weiterhin lernfähig?

**D.** Welche Gewichtsänderungen sind für die Erhaltung bereits erworbener Fähigkeiten tatsächlich erforderlich?

**E.** Welche synaptischen Änderungen besitzen keinen nachweisbaren funktionalen Nutzen?

**F.** Weshalb entstehen so viele negative Modulationsereignisse nach fehlenden Motorantworten?

**G.** Welche dieser Nichtantworten lassen sich durch negative sensorische Plastizität sinnvoll korrigieren?

**H.** Wann ist neutrales oder ausbleibendes Feedback eine sinnvolle Kontrollreaktion?

**I.** Welche Unterschiede bestehen zwischen neutralem und fehlendem Feedback?

**J.** Welche Eligibility- und Modulationszustände bleiben dabei erhalten?

**K.** Wie stark beeinflusst positive sensorische Konkurrenz die Stabilität anderer Fähigkeiten?

**L.** Welche Bedeutung hat synaptische Redundanz für die Bewertung notwendiger Veränderungen?

**M.** Lassen sich die zwölf historischen offenen Fälle weiter aufklären?

**N.** Welche Unterschiede zwischen notwendigen und unnötigen Änderungen sind ausschließlich lokal erkennbar?

**O.** Wo reichen die vorhandenen lokalen Informationen nicht aus?

**P.** Ist eine zusätzliche neuronale Lernregel tatsächlich notwendig?

**Q.** Welche konkrete Forschungsfrage ergibt sich für B.5.9?

---

## 25. Merge-Entscheidung

Der Abschlussbericht muss ausdrücklich enthalten:

**B.5.8 nach `main` mergen: Ja oder Nein.**

Eine positive Merge-Empfehlung ist nicht gleichbedeutend mit einer Produktionsfreigabe.

### Merge empfohlen, wenn

- Die Forschung reproduzierbar abgeschlossen ist.
- Alle Regressionstests bestanden wurden.
- Diagnose und normale neuronale Verarbeitung getrennt bleiben.
- Experimentelle Plastizitätssperren nicht automatisch aktiv werden.
- Notwendige negative Korrekturen nicht pauschal unterdrückt werden.
- S4 standardmäßig deaktiviert bleibt.
- Persistenz und Determinismus erhalten bleiben.
- Keine kritischen technischen Blocker existieren.

### Merge nicht empfohlen, wenn

- Ergebnisse nicht reproduzierbar sind.
- Die experimentelle Plastizitätssperre unbeabsichtigt normales Lernen verändert.
- Eine globale Erkennung erlernter Fähigkeiten zur Steuerung synaptischer Gewichte verwendet wird.
- Notwendige Lernvorgänge durch eine unbewiesene Regel verhindert werden.
- Regressionen oder Persistenzprüfungen fehlschlagen.

Ein negativer wissenschaftlicher Befund darf als gültiger Forschungsstand integriert werden.

---

## 26. Verbindliche Forschungsleitlinie

**Lernen können ist eine Fähigkeit. Etwas gelernt haben ist ein Zustand. Beides darf nicht verwechselt werden.**

Eine bestehende neuronale Fähigkeit kann genutzt werden, ohne dass ihre zugrunde liegenden Verbindungen ständig verändert werden müssen.

Die Verarbeitung neuer Erfahrungen ist nicht gleichbedeutend mit einer Veränderung synaptischer Gewichte.

Ein unverändertes Netzwerk kann weiterhin eine erlernte Fähigkeit besitzen.

Ein verändertes Netzwerk muss dadurch nicht zwangsläufig eine neue Fähigkeit erworben haben.

Die Fähigkeit zu lernen bleibt dennoch erforderlich, damit EVE auf neue Umweltbedingungen reagieren kann.

Wir suchen daher weder eine Architektur, die möglichst häufig Gewichte verändert, noch eine Architektur, die einmal erreichte Gewichte möglichst lange konserviert.

Wir suchen eine Architektur, deren synaptische Veränderungen aus tatsächlicher neuronaler Aktivität, Erfahrung und lokal verfügbarer Rückmeldung entstehen und deren Auswirkungen funktional überprüfbar sind.

**Das Ziel von B.5.8 ist zu untersuchen, wann neuronale Erfahrungen Veränderungen erfordern, wann bestehende Verbindungen ausreichen und wie EVE dabei ihre Fähigkeit zum weiteren Lernen erhalten kann.**

Eine neue neuronale Lernregel darf erst aus den nachgewiesenen Ursachen abgeleitet werden – nicht aus dem Wunsch nach einer höheren Erfolgsquote.
