# Entwicklungsauftrag 033 – Forschungsstufe B.5.9

## Lokale neuronale Entscheidungsmarge, synaptische Konkurrenz und die Entstehung sinnvoller Plastizität

**Projekt:** EVE – Emergent Virtual Evolution  
**Technologie:** C++17  
**Ausgangsbasis:** Vollständig integrierter Forschungsstand B.5.8  
**Forschungsbranch:** `codex/research-stage-b59-local-plasticity-causality`  
**Auftragstyp:** Kausale Grundlagenforschung, keine vorab freigegebene neue Produktionsregel

---

## 1. Verbindlicher Forschungsgrundsatz

**Lernen können ist eine Fähigkeit. Etwas gelernt haben ist ein Zustand. Beides darf nicht verwechselt werden.**

Diese Unterscheidung wurde in B.5.8 experimentell bestätigt.

Eine bereits vorhandene neuronale Fähigkeit kann ohne fortwährende synaptische Veränderungen verfügbar bleiben.

Gleichzeitig benötigt EVE für die Anpassung an neue Umweltbedingungen weiterhin die Möglichkeit, ihre synaptischen Verbindungen zu verändern.

Daraus ergibt sich die zentrale Anforderung:

**Synaptische Plastizität soll nicht grundsätzlich ein- oder ausgeschaltet werden. Untersucht werden muss, welche lokal vorhandenen Ereignisse eine konkrete synaptische Veränderung funktional begründen.**

Nicht jede neuronale Aktivität muss zu einer Gewichtsänderung führen.

Nicht jede ausbleibende Motorantwort rechtfertigt die Abschwächung bereits aktiver Synapsen.

Nicht jede erfolgreiche Handlung erfordert zusätzliche synaptische Verstärkung.

Aber ebenso gilt:

Eine negative Rückmeldung darf nicht pauschal ignoriert werden.

Eine funktional notwendige Verstärkung darf nicht unterdrückt werden, nur weil eine andere Beziehung bereits erlernt wurde.

**Die Forschungsaufgabe besteht darin, die tatsächlichen lokalen Voraussetzungen sinnvoller synaptischer Veränderungen zu untersuchen.**

---

## 2. Ausgangslage und bisherige Befunde

### 2.1 Ergebnisse B.5.7

B.5.7 zeigte:

- 9.444 negative sensorische Gewichtsänderungen in 48 ausgewählten nichtrobusten Netzen.
- 80,01 % der untersuchten Änderungen betrafen Synapsen, deren Einzelablation die aktuelle Motorantwort veränderte.
- 19,99 % zeigten bei Einzelablation keinen Einfluss auf die Motorantwort.
- 99,28 % der negativen Änderungen folgten einer fehlenden Motorantwort.
- Keine natürliche erfahrungsübergreifende Eligibility.
- Keine natürliche Mehrfachabschwächung innerhalb eines Modulationskontakts.

Die Untersuchung bestätigte eine wesentliche Grenze:

**Eligibility ist eine lokale zeitliche Lernberechtigung, aber kein eindeutiger Nachweis kausaler Verantwortlichkeit.**

### 2.2 Ergebnisse B.5.8

B.5.8 reproduzierte B.5.7 und untersuchte anschließend den Erhalt bestehender Fähigkeiten.

In 96 ausgewählten neuronalen Ausgangszuständen:

| Versuchsbedingung | Gleichzeitig robuste Netze |
|---|---:|
| Weitere Erfahrungen, Plastizität aktiv | 62/96 |
| Weitere Erfahrungen, Gewichtsschreibungen gesperrt | 90/96 |
| Umweltumkehr, Plastizität aktiv | 60/96 |
| Umweltumkehr, Gewichtsschreibungen weiterhin gesperrt | 0/96 |
| Umweltumkehr, Plastizität wieder aktiviert | 61/96 |

Daraus folgt:

- Erlernte Fähigkeiten können ohne fortwährende Gewichtsänderungen erhalten bleiben.
- Die pauschale Schreibsperre verhindert notwendiges Umlernen.
- Die spätere Reaktivierung ermöglicht erneute Anpassung.
- Die bestehende synaptische Plastizität kann sowohl nützlich als auch störend wirken.

### 2.3 Lokale Informationsgrenze

B.5.8 untersuchte 29 lokale Zustandssignaturen.

20 dieser Signaturen traten in mehreren unterschiedlichen globalen Funktionsklassen auf.

Damit ist anhand des bisher erfassten lokalen Informationsbestands keine sichere Trennung notwendiger, neutraler und schädlicher synaptischer Änderungen nachgewiesen.

Dies ist kein allgemeiner Unmöglichkeitsbeweis.

Es zeigt lediglich, dass die bisher untersuchten Informationen nicht ausreichen.

### 2.4 Offene Probleme

- Ursachen fehlender Motorantworten.
- Bedeutung relativer erregender und hemmender Beiträge.
- Unterschied zwischen bloßer neuronaler Beteiligung und entscheidendem Beitrag.
- Positive synaptische Konkurrenz.
- Zeitliche Abhängigkeit der neuronalen Verarbeitung.
- Zwölf weiterhin nicht vollständig erklärte historische B.5.6-Fälle.
- Fehlende sichere lokale Unterscheidung sinnvoller und störender Gewichtsanpassungen.

Diese Probleme bilden die Grundlage von B.5.9.

---

## 3. Zentrale Forschungsfrage

**Welche tatsächlich lokal verfügbaren neuronalen Informationen können anzeigen, ob ein synaptischer Beitrag eine postsynaptische Entscheidung beeinflusst und ob seine Veränderung unter einer gegebenen Modulation funktional gerechtfertigt ist?**

Dabei sind drei Ebenen strikt zu unterscheiden:

### Ebene A – Lokale neuronale Wirkung

Ein Eingang beeinflusst den Zustand und gegebenenfalls das Feuern seines Empfängerneurons.

### Ebene B – Wirkung auf die motorische Verarbeitung

Der lokale Beitrag beeinflusst möglicherweise die Weiterleitung einer Entscheidung bis zur Motorantwort.

### Ebene C – Wirkung auf zukünftiges Lernen

Eine synaptische Veränderung beeinflusst möglicherweise den späteren Erwerb, Erhalt oder Verlust einer Fähigkeit.

Aus A folgt nicht automatisch B.

Aus B folgt nicht automatisch C.

**B.5.9 muss diese Ebenen durch kontrollierte Experimente miteinander in Beziehung setzen, ohne sie gleichzusetzen.**

---

## 4. Tatsächlich vorhandene neuronale Informationen

Vor allen Experimenten ist die bestehende Implementierung technisch zu untersuchen.

Insbesondere:

- `Brainstem::receive_impulse_batch()`
- `Brainstem::create_eligibility()`
- `Runtime::process_impulse()`
- `Runtime::apply_local_modulation()`
- Lokale Ereignisgeschichte G3.
- Refraktär- und Recovery-Verarbeitung.
- S4-Subthreshold-Relaxation.
- Die vorhandenen Plastizitätsbeobachter.
- Die experimentelle Gewichtsschreibsperre aus B.5.8.

### 4.1 Vorhandene Eingangsinformation

Bei der Verarbeitung eines lokalen Impulsbündels sind bereits Informationen über folgende Größen verfügbar:

- Membranpotenzial vor dem Impulsbündel.
- Summe erregender Beiträge.
- Summe hemmender Beiträge.
- Nettoeingang.
- Feuerschwelle.
- Refraktärzustand.
- Tatsächlich eingetretenes Feuern.
- Unterdrücktes Feuern.
- Membranpotenzial nach der Verarbeitung.

Diese Informationen werden teilweise bereits im `ImpulseBatchResult` zusammengeführt.

### 4.2 Wichtige Einschränkung

Der bestehende Reset-Modus kann das Membranpotenzial unmittelbar nach einem Feuerereignis auf null setzen.

Eine relevante Entscheidungsmarge muss daher am tatsächlichen Verarbeitungszeitpunkt bestimmt werden.

**Aus dem später zurückgesetzten Membranpotenzial darf nicht nachträglich eine angebliche Entscheidungsmarge rekonstruiert werden.**

### 4.3 Lokale Information versus externe Diagnose

Die Forschung darf externe Zielinformationen und globale Funktionsprüfungen verwenden.

Diese Daten dürfen jedoch nicht in die lokale neuronale Plastizitätsentscheidung eingebaut werden.

Ein Neuron kennt nicht automatisch:

- Die richtige Motorantwort.
- Die Bedeutung eines sensorischen Reizes.
- Die korrekte Beziehung X oder Y.
- Den Erfolg der Gegenbeziehung.
- Eine globale Statistik bereits erlernter Fähigkeiten.

---

## 5. Forschungshypothesen

### H1 – Lokale Entscheidungsmarge

Die Entfernung eines synaptischen Beitrags verändert die Feuerentscheidung eines Neurons besonders häufig, wenn dessen Nettoerregung nahe der Feuerschwelle liegt.

### H2 – Relative synaptische Beiträge

Die funktionale Bedeutung einer Synapse hängt nicht nur von ihrem absoluten Gewicht ab, sondern vom Verhältnis zu den übrigen gleichzeitig wirksamen Eingängen.

### H3 – Erregende und hemmende Konkurrenz

Hemmende Beiträge können bestimmen, ob eine sensorische Verstärkung tatsächlich ein Feuerereignis verursacht oder ohne unmittelbare Wirkung bleibt.

### H4 – Redundanz

Mehrere synaptische Beiträge können gemeinsam eine Entscheidung tragen, obwohl keiner für sich allein notwendig ist.

### H5 – Unzureichende Erregung bei Nichtantwort

Ein erheblicher Teil ausbleibender motorischer Antworten entsteht durch nicht erreichte lokale Feuerschwellen und die nachfolgende Signalweiterleitung.

### H6 – Notwendige negative Korrektur

Negative Modulation kann bei ungünstiger lokaler Konkurrenz eine notwendige Anpassung verursachen, auch wenn zuvor keine Motorantwort zustande kam.

### H7 – Nicht erforderliche Gewichtsänderung

Eine Synapse kann zeitlich berechtigt sein, ohne dass ihre Veränderung für eine funktionale Verbesserung notwendig ist.

### H8 – Positive Konkurrenz

Positive sensorische Gewichtsänderungen können die lokale Entscheidungsmarge verändern und dadurch eine andere Beziehung beeinträchtigen.

### H9 – Zeitliche Konkurrenz

Der Einfluss eines synaptischen Beitrags hängt von der Reihenfolge, Gleichzeitigkeit und zeitlichen Überlappung neuronaler Eingänge ab.

### H10 – Zusätzliche lokale Information

Die tatsächlich während der Eingangsintegration beobachtbaren Größen könnten mehr Information über die funktionale Bedeutung einer Synapse enthalten als die in B.5.8 erfassten Signaturen am Modulationszeitpunkt.

### H11 – Grenzen der lokalen Kausalität

Auch eine vollständig bekannte lokale Feuerentscheidung muss nicht ausreichen, um zu bestimmen, ob eine synaptische Veränderung für das gesamte zukünftige Verhalten sinnvoll ist.

---

## 6. Phase A – Integration, Referenz und Reproduktion

### A1 – Integrationsbasis

B.5.8 muss vollständig nach `main` integriert sein.

Erst danach wird der neue B.5.9-Forschungsbranch erstellt.

Vorher darf B.5.9 nicht unabhängig auf einem veralteten `main` beginnen.

### A2 – Referenzreproduktion

Reproduziere mindestens:

- B.5.8 Discovery: 240/384 robuste Netze.
- B.5.8 unabhängige Bestätigung: 244/384 robuste Netze.
- B.5.7-Referenz.
- Die Stabilitätskontrolle P0/P1.
- Die Umweltumkehr P2/P3/P4.
- Die bisherige zeitliche Abhängigkeit.

### A3 – Unveränderte Produktionslogik

Für die kausale Diagnose gilt:

- Keine Änderung der normalen Plastizitätsregel.
- Keine Änderung der Feuergrenze.
- Keine Änderung der S4-Lebensdauer.
- Keine Änderung der Modulationspolarität.
- Keine automatische synaptische Stabilisierung.
- Keine zusätzliche globale Lernentscheidung.

---

## 7. Phase B – Lokale Entscheidungsmarge erfassen

Dies ist der technische Kern der Forschungsstufe.

### B1 – Vor dem Impuls

Erfasse den vollständigen relevanten lokalen Zustand unmittelbar vor der Eingangsintegration.

### B2 – Während des Impulsbündels

Erfasse:

- Alle eingegangenen Synapsen.
- Ihre jeweiligen tatsächlich angewendeten Beiträge.
- Summe erregender Eingänge.
- Summe hemmender Eingänge.
- Nettoeingang.
- Membranpotenzial vor der Integration.
- Effektive Membranlage unmittelbar vor der Feuerentscheidung.
- Feuerschwelle.
- Refraktärzustand.
- Ob ein Feuerereignis ausgelöst wurde.

### B3 – Definition der Entscheidungsmarge

Definiere eine diagnostische lokale Entscheidungsmarge als Differenz zwischen der wirksamen Membranlage unmittelbar vor der Feuerentscheidung und der Feuerschwelle.

Die Berechnung muss:

- Die tatsächliche Implementierung berücksichtigen.
- Unterlauf- und Überlaufbegrenzungen berücksichtigen.
- Membran-Clamping berücksichtigen.
- Refraktärzustände getrennt kennzeichnen.
- Den Zustand vor einem möglichen Membranreset verwenden.

Ein positiver Wert ist bei aktivem Refraktärzustand nicht automatisch ein tatsächliches Feuerereignis.

### B4 – Kein künstlicher Grenzwert

Es darf nicht vorab behauptet werden, dass beispielsweise eine Marge von 1 eine sinnvolle oder sinnlose synaptische Veränderung bedeutet.

Die Aussagekraft unterschiedlicher Margen ist experimentell zu untersuchen.

---

## 8. Phase C – Kausale Bedeutung einzelner Eingänge

### C1 – Identische Zustandskopien

Erzeuge vollständige Runtime-Kopien unmittelbar vor einem lokalen Eingangsereignis.

### C2 – Einzelbeitrag entfernen

Entferne in einer Diagnosekopie ausschließlich den betreffenden synaptischen Beitrag.

### C3 – Lokale Auswertung

Prüfe:

- Verändert sich die lokale Membranlage?
- Verändert sich die Entscheidungsmarge?
- Verändert sich das Feuern?
- Verändert sich die lokale Eligibility?
- Verändert sich die spätere Verarbeitung?

### C4 – Positive und negative Beiträge

Führe diese Untersuchungen getrennt durch für:

- Erregende Synapsen.
- Hemmende Synapsen.
- Mehrere gleichzeitige erregende Beiträge.
- Gemischte erregende und hemmende Beiträge.

### C5 – Redundanz

Eine Einzelablation ohne Änderung des Feuerns ist kein ausreichender Beleg für funktionale Bedeutungslosigkeit.

Prüfe deshalb ausgewählte Kombinationen.

### C6 – Ergebnis

Unterscheide:

1. Beitrag verändert nur das Membranpotenzial.
2. Beitrag verändert die Entscheidungsmarge.
3. Beitrag ist einzeln notwendig für ein Feuerereignis.
4. Beitrag ist Teil einer gemeinsam notwendigen Kombination.
5. Beitrag verändert erst einen späteren lokalen Zustand.
6. Kein kausaler Einfluss im untersuchten Beobachtungsfenster nachweisbar.

---

## 9. Phase D – Nichtfeuerentscheidungen

Bisher stand häufig das tatsächlich ausgelöste Feuern im Vordergrund.

B.5.9 muss ausdrücklich auch Nichtfeuerentscheidungen untersuchen.

### D1 – Vorhandene Eingänge ohne Feuern

Erfasse Neuronen, die Eingänge erhalten, aber ihre Feuerschwelle nicht erreichen.

### D2 – Gründe

Unterscheide:

- Zu geringe erregende Beiträge.
- Überwiegende Hemmung.
- Refraktärunterdrückung.
- Zeitlich getrennte Eingänge.
- Subthreshold-Relaxation.
- Bereits erfolgte Rücksetzung durch ein früheres Feuerereignis.

### D3 – Lokale Aktivität

Ein Neuron, das nicht feuert, kann trotzdem eine neuronale Erfahrung verarbeitet haben.

Es darf daher nicht automatisch als inaktiv klassifiziert werden.

### D4 – Eligibility

Prüfe, ob und unter welchen Bedingungen in solchen Situationen überhaupt lokale Eligibility entsteht.

### D5 – Negative Modulation

Untersuche, welche zuvor beteiligten Eingänge bei späterer negativer Modulation tatsächlich abgeschwächt werden.

### D6 – Kausale Frage

Führt die Abschwächung eines Beitrags nach einer Nichtantwort dazu, dass zukünftige Antworten wahrscheinlicher oder unwahrscheinlicher werden?

Diese Frage darf nicht anhand des bloßen Vorzeichens der Gewichtsänderung beantwortet werden.

---

## 10. Phase E – Synaptische Konkurrenz

### E1 – Relative Eingangsbeiträge

Untersuche die relative Bedeutung einzelner Synapsen innerhalb der tatsächlich gemeinsam aktiven Eingänge.

### E2 – Inhibitorische Konkurrenz

Bestimme, unter welchen Umständen hemmende Beiträge:

- Feuern verhindern.
- Feuern zeitlich verschieben.
- Eine andere erregende Verbindung relativ wirksamer machen.
- Übermäßige gemeinsame Aktivierung verhindern.

### E3 – Positive Verstärkung

Untersuche Fälle, in denen eine positive Gewichtsänderung die Feuerentscheidung verändert.

### E4 – Auswirkungen auf andere Beziehungen

Prüfe, ob dieselbe Änderung:

- Die aktuelle Beziehung verbessert.
- Die Gegenbeziehung beschädigt.
- Beide Beziehungen beeinflusst.
- Keine unmittelbare Änderung verursacht.

### E5 – Kausale Einzel- und Kombinationsproben

Verwende vollständige Kopien vor der relevanten Gewichtsänderung.

Eine bloße zeitliche Koinzidenz zwischen Verstärkung und Fähigkeitsverlust darf nicht als Kausalnachweis gelten.

---

## 11. Phase F – Eligibility und lokale Entscheidungsmarge

### F1 – Tatsächliche Kandidatenentstehung

Untersuche, welche synaptischen Beiträge beim Erzeugen eines Eligibility-Eintrags vorhanden waren.

### F2 – Zuordnung

Erfasse für jeden Kandidaten:

- Tatsächlich angewendeten Beitrag.
- Lokale Entscheidungsmarge.
- Erregenden oder hemmenden Effekt.
- Zeitpunkt seiner Aktivität.
- Zugehörigkeit zum aktuellen G3-Eintrag.
- Tatsächliche Veränderung bei späterer Modulation.

### F3 – Kandidaten mit unterschiedlicher Wirkung

Prüfe, ob Eligibility-Kandidaten mit vergleichbarer lokaler Aktivität unterschiedliche kausale Beiträge zur Feuerentscheidung besitzen.

### F4 – Nichtfeuer und Eligibility

Prüfe, ob für wichtige subthreshold aktive Eingänge keine Eligibility entsteht, obwohl diese für eine spätere neuronale Anpassung relevant sein könnten.

Dies ist zunächst eine Diagnosefrage.

Es darf daraus nicht automatisch eine neue Eligibility-Regel abgeleitet werden.

### F5 – Forschungsergebnis

Untersuche, ob die Kombination aus Eligibility und lokaler Entscheidungsmarge den tatsächlichen kausalen Beitrag besser beschreibt als Eligibility allein.

---

## 12. Phase G – Vorhersage nützlicher und schädlicher Gewichtsänderungen

### G1 – Lokales Informationsinventar

Vergleiche:

**L0 – B.5.8-Signatur**

Die bisherige lokale Beschreibung.

**L1 – Erweiterte Eingangsinformation**

B.5.8-Signatur plus erregende und hemmende Beiträge.

**L2 – Entscheidungsmarge**

L1 plus lokale Entscheidungsmarge und tatsächliche Feuerentscheidung.

**L3 – Zeitliche Information**

L2 plus lokale zeitliche Reihenfolge und Eligibility-Bezug.

**L4 – Konkurrenzinformation**

L3 plus gleichzeitig aktive konkurrierende Eingänge und ihre Beiträge.

### G2 – Vergleichbarkeit

Alle Varianten müssen anhand derselben Ausgangszustände und derselben extern ermittelten funktionalen Folgen bewertet werden.

### G3 – Vorhersageziel

Untersuche, ob lokale Informationen zwischen folgenden diagnostischen Kategorien unterscheiden können:

- Änderung unmittelbar funktional nützlich.
- Änderung unmittelbar funktional schädlich.
- Änderung unmittelbar neutral.
- Änderung erst in späterer natürlicher Fortsetzung wirksam.

### G4 – Verbotene Information

Das externe Diagnoseergebnis darf nicht selbst Bestandteil des lokalen Merkmalsvektors sein.

### G5 – Wissenschaftliche Grenze

Eine gute statistische Vorhersage ist noch keine kausale Lernregel.

Es ist zusätzlich nachzuweisen, dass eine darauf beruhende lokale Veränderung tatsächlich einen funktionalen Vorteil verursacht.

---

## 13. Phase H – Lokale Ununterscheidbarkeit prüfen

### H1 – Gleiche lokale Zustände

Suche Fälle mit identischen lokal verfügbaren Zuständen und unterschiedlichen späteren funktionalen Folgen.

### H2 – Vergleich der Informationsstufen

Bestimme, wie häufig solche Mehrdeutigkeiten unter L0, L1, L2, L3 und L4 auftreten.

### H3 – Unvollständige Zustandsbeschreibung

Unterscheide:

- Identische erfasste Merkmalsvektoren.
- Tatsächlich identische vollständige lokale Zustände.

Diese dürfen nicht gleichgesetzt werden.

### H4 – Informationsgrenze

Falls identische lokale Zustände unterschiedliche globale Folgen besitzen, ist für diese Zustände eine sichere Unterscheidung anhand genau dieser Informationen nicht möglich.

### H5 – Keine Scheinerklärung

Es darf nicht nachträglich behauptet werden, dass ein unbekannter lokaler Mechanismus den Unterschied wahrscheinlich erklären würde.

Entweder lässt sich eine Information nachweisen oder die Erklärung bleibt offen.

---

## 14. Phase I – Erhalt vorhandener Fähigkeiten

B.5.8 hat gezeigt, dass der Erhalt bestehender Fähigkeiten nicht fortwährende Plastizität erfordert.

B.5.9 muss diesen Befund als Kontrolle erhalten.

### I1 – Bereits erworbene Beziehungen

Wähle neuronale Zustände mit nachgewiesener gemeinsamer Abrufbarkeit.

### I2 – Natürliche Fortsetzung

Verarbeite weitere Erfahrungen bei aktiver Plastizität.

### I3 – Diagnostische Schreibsperre

Verwende die bestehende experimentelle Schreibsperre ausschließlich als Kontrollbedingung.

Sie darf nicht anhand eines erkannten Lernerfolgs automatisch aktiviert werden.

### I4 – Forschungsfrage

Untersuche, ob bestimmte lokale Entscheidungsmargen oder Konkurrenzmuster mit stabilen Phasen ohne notwendige Gewichtsschreibungen zusammenhängen.

### I5 – Keine Produktionsfunktion

Die Diagnose darf nicht zu einer globalen Regel werden, die erfolgreich gelernte Netze schützt.

---

## 15. Phase J – Lernfähigkeit und Umweltumkehr

### J1 – Erlernte Ausgangsfähigkeit

Beginne mit bereits erworbenen Beziehungen.

### J2 – Umweltveränderung

Ändere die äußere Bewertung der Reiz-Handlungs-Beziehungen.

### J3 – Plastizität aktiv

Untersuche, welche lokalen Änderungen zum Erwerb der neuen Beziehung beitragen.

### J4 – Kontrollierte Nichtveränderung

Vergleiche mit diagnostischen Kopien, bei denen ausgewählte Gewichtsänderungen unterdrückt werden.

### J5 – Erhaltene notwendige Korrekturen

Jede mögliche neue lokale Regel muss zeigen, dass sie notwendige Anpassungen nicht verhindert.

### J6 – Geschützte Exploration

Der vorhandene explorative Mechanismus bleibt unangetastet.

Keine künstlich erzwungene richtige Handlung.

---

## 16. Phase K – Die zwölf offenen B.5.6-Fälle

Diese Fälle bleiben verpflichtend.

### K1 – Frühere Ursachen

Die B.5.8-Gegenproben begannen bei mehreren Fällen sehr spät, sodass kaum noch relevante Gewichtsschreibungen untersucht werden konnten.

### K2 – Frühere Zeitpunkte

Rekonstruiere deshalb möglichst die vollständige Entwicklung ab dem ersten gemeinsamen Erwerb.

### K3 – Lokale Konkurrenz

Untersuche:

- Erregende Eingänge.
- Hemmende Beiträge.
- Entscheidungsmargen.
- Eligibility.
- Positive und negative Modulation.
- Membranzustände.
- Zeitliche Ereignisfolgen.

### K4 – Ursache vor Endzustand

Die Analyse soll konkrete Verlustereignisse untersuchen, nicht lediglich verschiedene Endzustände vergleichen.

### K5 – Offene Ergebnisse

Bleiben Fälle ungeklärt, werden sie ausdrücklich als offen dokumentiert.

---

## 17. Phase L – Unterschiedliche zeitliche Bedingungen

Mindestens:

- Fest 12.
- Fest 16.
- Fest 32.
- Unregelmäßige Erfahrungsabstände.
- Kurze Reizfolgen.

### L1 – Zeitliche Verarbeitung

Erfasse:

- Reihenfolge lokaler Eingänge.
- Gleichzeitige Impulsbündel.
- Membranpotenziale.
- Refraktärzustände.
- Recovery.
- S4.
- Eligibility.
- Modulation.

### L2 – Kein globaler Tick

Die Forschung bleibt vollständig ereignisgesteuert.

### L3 – Keine Optimierung von S4

S4-d16 bleibt unverändert und ausschließlich experimentell.

### L4 – Zeitliche Überlappung

Falls überlappende Reizfolgen untersucht werden, müssen sie als solche implementiert und dokumentiert werden.

Kurze, aber weiterhin vollständig getrennte Episoden dürfen nicht als echte überlappende Umweltinteraktion bezeichnet werden.

---

## 18. Phase M – Bedingte minimale lokale Forschungsvariante

**Eine neue Lernregel wird nicht vorab freigegeben.**

Erst nach positiver kausaler Evidenz darf eine minimale lokale Variante vorgeschlagen werden.

### Voraussetzungen

1. Eine konkrete schädliche Änderungsklasse wurde nachgewiesen.
2. Sie ist anhand tatsächlich lokal verfügbarer Informationen unterscheidbar.
3. Die unterscheidende Information ist zum tatsächlichen neuronalen Entscheidungszeitpunkt verfügbar.
4. Eine minimal isolierte Änderung der Plastizitätsverarbeitung verursacht einen positiven Effekt.
5. Notwendige positive und negative Korrekturen bleiben erhalten.
6. Die Gegenbeziehung wird nicht systematisch beeinträchtigt.
7. Umweltumkehr bleibt möglich.
8. Die Variante besitzt einen standardmäßig deaktivierten Schalter.
9. Die unveränderte Referenz wird mit deaktiviertem Schalter exakt reproduziert.
10. Eine unabhängige Bestätigung liegt vor.

### Denkbare Forschungsrichtung

Die Stärke oder Wirksamkeit einer lokalen synaptischen Veränderung könnte von einer kausal belegten lokalen Eingangs- oder Konkurrenzinformation abhängen.

Dies ist ausschließlich eine mögliche Hypothese.

Es darf nicht vorab festgelegt werden, dass:

- Eine kleine Entscheidungsmarge automatisch Lernen verlangt.
- Eine große Entscheidungsmarge automatisch Lernen verhindert.
- Ein feuerndes Neuron immer verstärkt werden muss.
- Ein nichtfeuerndes Neuron immer abgeschwächt werden muss.

Die tatsächliche Richtung muss aus den Experimenten hervorgehen.

### Nicht freigegeben

- Globale Gewichtsschreibsperre als Lernmechanismus.
- Erkennung eines fertig gelernten Netzes.
- Erfolgsabhängiger Schutz einzelner Synapsen.
- Rollback früher erfolgreicher Gewichte.
- Externe Zielinformationen in der Plastizität.
- Technisch richtige Motorantworten.
- Pauschale Unterdrückung positiver Modulation.
- Pauschale Unterdrückung negativer Modulation.
- Winner-take-all.
- Globale Lernzustände.
- Neue periodische Aktualisierungen.
- Optimierung der S4-Zeitkonstante.
- Beliebige Parameterbestwertsuche.

---

## 19. Kontrollgruppen

| Gruppe | Inhalt |
|---|---|
| K0 | Unveränderte B.5.8-Referenz |
| K1 | B.5.8-Lokalsignatur L0 |
| K2 | Erweiterte Eingangsinformation L1 |
| K3 | Lokale Entscheidungsmarge L2 |
| K4 | Zeitliche Zusatzinformation L3 |
| K5 | Lokale Konkurrenzinformation L4 |
| K6 | Einzelne erregende Ablation |
| K7 | Einzelne hemmende Ablation |
| K8 | Kombinierte Ablation |
| K9 | Positive Gewichtsänderung |
| K10 | Negative Gewichtsänderung |
| K11 | Diagnostisch keine Gewichtsänderung |
| K12 | Nichtfeuer- und Nichtantwort-Diagnose |
| K13 | Normale stabile Fortsetzung |
| K14 | Diagnostische Gewichtsschreibsperre |
| K15 | Aktives Umlernen |
| K16 | Gesperrtes Umlernen |
| K17 | Reaktivierte Plastizität |
| K18 | Unterschiedliche Zeitraster |
| K19 | Zwölf offene historische Fälle |
| K20 | No-Learning |
| K21 | Gegebenenfalls neue lokale Variante |
| K22 | Dieselbe Variante deaktiviert |

K21 und K22 werden nur implementiert, wenn die kausalen Freigabevoraussetzungen erfüllt sind.

---

## 20. Versuchsumfang und statistische Anforderungen

### Discovery

Mindestens:

- 64 Seeds je Stromfamilie und Zuordnung.
- Drei unabhängige Zufallsstromfamilien.
- Normale und inverse Zuordnung.
- 512 natürliche Trainingsepisoden je Netz.

### Bestätigung

Mindestens weitere 64 bisher unbenutzte Seeds je zentraler Kombination.

Die Bestätigungsdaten dürfen nicht vorher zur Auswahl von Signaturen, Grenzwerten oder Varianten benutzt werden.

### Kausalstichprobe

Die Auswahl detaillierter Ereignisse ist vor Kenntnis der betreffenden Interventionsergebnisse festzulegen.

Sie muss enthalten:

- Robuste Netze.
- Nichtrobuste Netze.
- Korrekte und falsche Antworten.
- Nichtantworten.
- Erregende und hemmende Beiträge.
- Einzelne und kombinierte kausale Wirkungen.
- Positive und negative synaptische Änderungen.

### Statistische Auswertung

Erforderlich sind:

- Gepaarte Vergleiche.
- Seedbasierte Clusterung.
- 95-%-Bootstrap-Konfidenzintervalle.
- Getrennte Auswertung der Zufallsfamilien.
- Getrennte normale und inverse Zielzuordnung.
- Vollständige Angabe der Fallzahlen.
- Unterscheidung von Ereignissen und unabhängigen Netzen.
- Positive, negative und neutrale Änderungsfolgen.
- Retention.
- Umweltumkehr.
- Erhalt bereits erlernter Fähigkeiten.

Einzelne synaptische Ereignisse desselben Netzes dürfen nicht als unabhängige Versuchseinheiten gezählt werden.

---

## 21. Erfolgskriterien

### Wissenschaftlicher Erfolg

B.5.9 ist erfolgreich, wenn:

1. Lokale Entscheidungsmargen korrekt und ereignisnah erfasst werden.
2. Erregende und hemmende Beiträge getrennt ausgewertet werden.
3. Einzelne und kombinierte kausale Beiträge untersucht werden.
4. Nichtfeuerentscheidungen ausdrücklich berücksichtigt werden.
5. Der Zusammenhang zwischen Eligibility und tatsächlicher lokaler Feuerentscheidung untersucht wird.
6. Die Informationsstufen L0 bis L4 vergleichbar ausgewertet werden.
7. Lokale Mehrdeutigkeit quantifiziert wird.
8. Positive und negative synaptische Veränderungen symmetrisch untersucht werden.
9. Bereits erlernte Fähigkeiten und erneute Lernfähigkeit getrennt bewertet werden.
10. Die zwölf offenen Fälle weiter aufgeklärt oder nachvollziehbar offen gelassen werden.
11. Unabhängige Bestätigungsdaten vorliegen.

Eine neue neuronale Lernfunktion ist für den wissenschaftlichen Abschluss nicht erforderlich.

### Freigabe einer neuen Regel

Es gelten weiterhin:

- Mindestens 75 % E1 für X.
- Mindestens 75 % E1 für Y.
- Mindestens 48/64 gleichzeitig robuste Seeds je Stromfamilie und Zuordnung.
- Mindestens 15 Prozentpunkte Lernvorteil gegenüber No-Learning.
- Vollständig positives 95-%-Konfidenzintervall.
- Eigener, kausal nachgewiesener Vorteil gegenüber der unveränderten Referenz.
- Unabhängige Bestätigung.
- Erhaltene Retention.
- Erhaltenes Umlernen.
- Geschützte Exploration.
- Deterministische Persistenz.
- Begrenzter Ressourcenbedarf.

Ein wissenschaftlich interessanter lokaler Zusammenhang ist noch keine Freigabe einer neuen Produktionsregel.

---

## 22. Technische Validierung

Alle bisherigen **665 Kerntests** müssen erhalten bleiben.

Zusätzliche Tests mindestens für:

- Exakte lokale Eingangsbeiträge.
- Erregende und hemmende Summen.
- Nettoeingang.
- Entscheidungsmarge.
- Feuerentscheidung vor Membranreset.
- Nichtfeuerentscheidung.
- Refraktärunterdrückung.
- Membran-Clamping.
- Gleichzeitige Impulsbündel.
- Unterschiedliche Eingangsreihenfolgen.
- Einzelne erregende Ablation.
- Einzelne hemmende Ablation.
- Kombinierte Ablation.
- Redundante synaptische Beiträge.
- Positive lokale Gewichtsänderung.
- Negative lokale Gewichtsänderung.
- Null-Gewichtsänderung.
- Eligibility-Entstehung.
- Eligibility-Verbrauch.
- Modulationszustellung.
- Lokale Konkurrenzsignaturen.
- Keine externen Zielinformationen in der lokalen Verarbeitung.
- Unveränderte Schreibsperrenkontrolle.
- Normale und inverse Zuordnung.
- Umweltumkehr.
- Geschützte Exploration.
- S4 standardmäßig deaktiviert.
- Persistenz aller neu eingeführten Forschungszustände.
- Exakte deterministische Fortsetzung.
- Gültige neuronale Zustände.

Neue diagnostische Beobachter dürfen keine neuronalen Zustände verändern.

---

## 23. Architektur- und Ressourcenanforderungen

Unverändert gelten:

- C++17.
- Ereignisgesteuerte Verarbeitung.
- Kein globaler Tick.
- Keine periodische Aktualisierung sämtlicher Neuronen.
- Binäre Feuerereignisse.
- Gerichtete gewichtete Synapsen.
- Lokale neuronale Integration.
- Erregende und hemmende Eingänge.
- G3.
- Eligibility.
- Drei-Faktor-Plastizität.
- Geschützte Exploration.
- Kein globales Zielwissen in der Plastizität.
- Keine automatische Sicherung erlernter Gewichte.
- Keine automatische Wiederherstellung früherer Zustände.

### Ressourcenmessung

Erfasse:

- CPU-Zeit.
- Zusätzliche Ereignisse.
- Speicherbedarf je Neuron.
- Speicherbedarf je Synapse.
- Anzahl diagnostischer Kopien.
- Maximale Ereigniswarteschlange.
- Geschätzten Runtime-Speicher.
- Persistenzgröße.
- Rohdatengröße.

Bevorzuge vorübergehende lokale Diagnosewerte gegenüber permanent gespeicherten zusätzlichen Zuständen, sofern sie die Forschungsfrage vollständig beantworten.

Große Forschungsdaten sind komprimiert abzulegen.

---

## 24. Lieferumfang

Nova-EVE erstellt:

1. Reproduktion der B.5.8-Referenz.
2. Technische Analyse der lokalen Eingangsintegration.
3. Definition und Validierung der Entscheidungsmarge.
4. Vollständige Diagnostik erregender und hemmender Beiträge.
5. Kausale Einzelablationen.
6. Kausale Kombinationsablationen.
7. Untersuchung lokaler Redundanz.
8. Untersuchung von Nichtfeuerentscheidungen.
9. Analyse fehlender Motorantworten.
10. Verbindung zwischen lokaler Integration und Eligibility.
11. Vergleich der Informationsstufen L0 bis L4.
12. Untersuchung lokaler Mehrdeutigkeit.
13. Kausale Untersuchung negativer Gewichtsänderungen.
14. Kausale Untersuchung positiver Konkurrenz.
15. Stabilitätskontrollen ohne weitere Gewichtsänderungen.
16. Aktive und unterdrückte Plastizität als diagnostische Kontrollen.
17. Umweltumkehr und Reaktivierung.
18. Untersuchung der zwölf historischen offenen Fälle.
19. Unterschiedliche Zeitbedingungen.
20. Unabhängige Bestätigung.
21. Gegebenenfalls minimal begründete lokale Forschungsvariante.
22. Vollständige Abschaltkontrolle.
23. Regressionstests.
24. Persistenzprüfung.
25. Ressourcenanalyse.
26. Vollständige Rohdaten und statistische Auswertung.
27. Forschungsbericht B.5.9.
28. Technische Kausalanalyse.
29. Architekturentscheidung.
30. Entscheidungsvorlage B.5.10.
31. Explizite Merge-Empfehlung.

---

## 25. Abschlussfragen

**A.** Welche erregenden und hemmenden Eingänge bestimmen tatsächlich eine lokale Feuerentscheidung?

**B.** Welche Bedeutung besitzt die lokale Entscheidungsmarge?

**C.** Wie unterscheiden sich lokal notwendige, redundante und nicht nachweislich wirksame synaptische Beiträge?

**D.** Welche Informationen fehlen bei einer Nichtfeuerentscheidung?

**E.** Lassen sich bestimmte fehlende Motorantworten auf konkrete lokale Integrationsbedingungen zurückführen?

**F.** Wie hängen lokale Feuerentscheidungen mit der späteren Eligibility zusammen?

**G.** Können lokale Entscheidungsmargen notwendige von schädlichen Gewichtsänderungen besser unterscheiden?

**H.** Welche zusätzliche Information liefert die relative erregende und hemmende Konkurrenz?

**I.** Welche Information liefert die zeitliche Reihenfolge der Eingänge?

**J.** Welche Unterschiede bestehen zwischen den lokalen Informationsstufen L0 bis L4?

**K.** Welche Mehrdeutigkeiten bleiben trotz zusätzlicher lokaler Information bestehen?

**L.** Welche positiven Gewichtsänderungen sind tatsächlich kausal für spätere Fähigkeitsverluste?

**M.** Welche negativen Gewichtsänderungen sind für Korrektur und Umlernen erforderlich?

**N.** Können erlernte Fähigkeiten weiterhin ohne dauernde Gewichtsschreibungen erhalten bleiben?

**O.** Welche lokale Information könnte einer Synapse erlauben, bei bestimmten Erfahrungen unverändert zu bleiben?

**P.** Ist eine solche Unterscheidung mit den vorhandenen neuronalen Mechanismen überhaupt möglich?

**Q.** Welche der zwölf historischen offenen Fälle lassen sich weiter aufklären?

**R.** Ist eine neue lokale Lernregel kausal gerechtfertigt?

**S.** Welche Forschungsfrage folgt für B.5.10?

---

## 26. Merge-Entscheidung

Der Abschlussbericht muss ausdrücklich enthalten:

**B.5.9 nach `main` mergen: Ja oder Nein.**

### Merge empfohlen, wenn

- Die Forschung reproduzierbar abgeschlossen wurde.
- Alle Regressionstests bestanden wurden.
- Die lokale Entscheidungsmarge korrekt erfasst wird.
- Neue Diagnosen keine normale neuronale Verarbeitung verändern.
- Keine globalen Erfolgslabels in die Plastizität gelangen.
- Neue Forschungsvarianten ausschließlich bei nachgewiesener Kausalität eingeführt werden.
- Der Produktionsstandard unverändert bleibt.
- Persistenz und Determinismus erhalten bleiben.
- Keine kritischen Integrationsprobleme bestehen.

### Merge nicht empfohlen, wenn

- Die Experimente nicht reproduzierbar sind.
- Diagnosen das neuronale Verhalten unbeabsichtigt verändern.
- Eine lokale Lernregel externe richtige Antworten verwendet.
- Die experimentelle Gewichtsschreibsperre als automatische Stabilitätsfunktion missbraucht wird.
- Notwendige Lernreaktionen ohne Begründung verhindert werden.
- Regressionen oder Persistenzprüfungen fehlschlagen.

Eine wissenschaftlich abgeschlossene Forschungsstufe darf auch ohne neue Produktionsregel integriert werden.

---

## 27. Verbindliche Forschungsleitlinie

**Lernen können ist eine Fähigkeit. Etwas gelernt haben ist ein Zustand.**

Die Erhaltung einer Fähigkeit benötigt nicht zwangsläufig fortwährende synaptische Veränderungen.

Die Verarbeitung einer Erfahrung erfordert nicht zwangsläufig eine neue synaptische Anpassung.

Gleichzeitig muss ein neuronales System bei veränderten Umweltbedingungen weiterhin lernen können.

Die Lösung besteht nicht darin, erlernte Verbindungen nachträglich technisch zu schützen.

Stattdessen soll erforscht werden, welche tatsächlich lokal verfügbaren Ereignisse eine synaptische Veränderung rechtfertigen.

Eine lokale Feuerentscheidung ist jedoch noch keine globale richtige Handlung.

Eine zeitlich gültige Eligibility ist noch kein Kausalnachweis.

Eine statistische Korrelation ist noch keine Lernregel.

Eine höhere Erfolgsquote ist noch kein Beleg für einen geeigneten Mechanismus.

**B.5.9 soll untersuchen, ob die tatsächliche lokale neuronale Dynamik genügend Information über synaptische Beiträge liefert, um Lernen und Nichtlernen aus dem neuronalen Verhalten selbst entstehen zu lassen.**

Wenn diese Information nicht ausreicht, ist das ausdrücklich als Forschungsergebnis festzuhalten.

Es darf keine scheinbar plausible Ersatzregel konstruiert werden, nur um einen positiven Befund zu erzwingen.
