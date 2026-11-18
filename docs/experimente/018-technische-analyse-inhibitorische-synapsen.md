# Technische Analyse – inhibitorische Synapsen in B.4.6

## 1. Bewertete Modelle

### Variante I: signiertes Gewicht

Ein negatives Gewicht wäre kompakt und die Membranwirkung unmittelbar. Es
vermischt jedoch zwei verschiedene Bedeutungen: synaptische Wirkungsart und
lernbare Wirkstärke. Nullgewicht-Verbindungen verlieren ihre inhibitorische
Identität; bestehende Nichtnegativitäts-, Budget-, Auswahl- und
Persistenzannahmen müssten an vielen Stellen neu interpretiert werden. Eine
negative Lernmodulation wäre außerdem leicht mit einem negativen neuronalen
Beitrag zu verwechseln.

### Variante II: getrennte Wirkungsart

Die Synapse speichert `SynapticEffect::{excitatory,inhibitory}` und weiterhin
eine nichtnegative Stärke. Ein erregender Beitrag addiert, ein hemmender
Beitrag subtrahiert lokal. Die Identität bleibt auch bei Gewicht null erhalten.
Gewichtsbounds, Nullgewicht-Plastizität und die Trennung von Aktivitätswirkung
und drittem Lernfaktor bleiben eindeutig.

Das Feuerereignis selbst bleibt absichtlich binär und trägt keine
„inhibitorische“ Sonderklasse. Erst die empfangende Synapse bestimmt die
Wirkung. Die Diagnose verbindet deshalb das gewöhnliche Senderfeuer mit dem
angewandten inhibitorischen Impuls und dessen lokaler Stärke.

## 2. Entscheidung

B.4.6 verwendet Variante II. Die zusätzliche Eigenschaft ist allgemein für
alle Synapsen verfügbar und enthält keine Motor- oder Reizsemantik.
Bestehende Synapsen bleiben standardmäßig erregend. Zustandsformat 13 speichert
Wirkungsart, Batch-Konfiguration, Plastizitätsmodus und neue Zähler; ältere
Zustände werden als rein erregend geladen.

## 3. Lokale Integration

Ein einzelner inhibitorischer Impuls reduziert das Membranpotenzial höchstens
bis null. Er erzeugt selbst kein Feuerereignis. Für technisch gleichzeitig
geplante Eingänge kann `batch_simultaneous_inputs` aktiviert werden. Alle
Beiträge eines lokalen Empfängers werden kanonisch nach Synapsen-ID geordnet,
als Summe integriert und erst danach gegen die Feuergrenze geprüft.

Das Verfahren ist ereignisgetrieben und benötigt keinen globalen Tick. Die
Batch-Grenze bezeichnet ausschließlich eine bereits technisch gleiche
Ereigniswelle. Neu entstehende Feuerereignisse bilden die nächste Welle.
Vertauschte Einfügungs- und Queue-Reihenfolge verändert das neuronale Ergebnis
nicht.

Die isolierte Gegenkontrolle zeigt, warum die Batch-Grenze nötig ist: Bei
voraufgeladenem Membranwert kann eine sequenzielle Verarbeitung von Erregung
und Hemmung abhängig von der Reihenfolge feuern oder nicht feuern. Dieser
Effekt wird in `isolated-inhibition.csv` sichtbar ausgewiesen und nicht als
Lernleistung gewertet.

## 4. Plastizität

Zuerst wird die vorhandene Regel unverändert geprüft (`shared`): Positive
Modulation erhöht die Stärke jedes berechtigten Beitrags, negative Modulation
senkt sie – unabhängig von dessen Wirkungsart. Zusätzlich untersucht B.4.6
eine begründete Beitragssignatur (`contribution_aware`): Für inhibitorische
Kandidaten wird die Modulationsrichtung invertiert.

Beide Regeln verwenden ausschließlich lokale `active_phase`-Eligibility.
Nullgewicht-Hemmung bleibt auswählbar. Die Umwelt wählt keine konkrete
inhibitorische Verbindung. Feste Hemmung wird durch Plastizitätsschutz von
lernbarer Hemmung getrennt.

## 5. Sicherheitsgrenzen

- Synaptische Stärken bleiben in `[0,3]`.
- Membranwerte werden durch Hemmung nicht negativ.
- Hemmung erzeugt weder eigene Motorentscheidung noch Ereignisschleife.
- Wachstum bleibt in der gesamten B.4.6-Matrix deaktiviert.
- Der neue Batch-Modus ist standardmäßig aus; B.4.5 und frühere Versuche
  behalten ihre Ereignissemantik.
- Wirkungsart, Gewichte, Metriken, offene Ereignisse und Fortsetzung werden in
  64 Zustandsproben exakt wiederhergestellt.

## 6. Experimentelles Konkurrenzmotiv

Zwei gleichberechtigte Entscheidungsneuronen erregen jeweils ihren eigenen
Motorweg und hemmen symmetrisch den konkurrierenden Motorweg. Beide Seiten
verwenden dieselbe Schwelle, Stärke und Lernregel. Vertauschte Konstruktion
prüft Queue-Bias; vertauschte Zielzuordnung prüft verbleibende technische
Handlungsasymmetrie. Das Motiv kennt die richtige Zuordnung nicht.
