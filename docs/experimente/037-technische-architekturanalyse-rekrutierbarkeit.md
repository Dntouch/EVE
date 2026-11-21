# Technische Architekturanalyse – B.5.13

## Bestand

`Runtime::enqueue_exploration()` und `Brainstem::select_exploration()` können
auch vollständig unverbundene Neuronen auswählen. Bei
`processing_neurons_only` werden die expliziten Reservierungen korrekt
ausgeschlossen. Lokalisation und Verteilung verändern nur die Auswahl; ein
bestehender synaptischer Pfad ist keine Voraussetzung.

`Runtime::process_fire()` versucht bei aktivem Growth nach dem Senden über die
bereits vorhandenen Ausgänge strukturelles Wachstum. Für einen Sender mit
`k` Ausgängen beträgt die Versuchswahrscheinlichkeit `1/(1+k)`. Bei `k=0` ist
der Versuch garantiert. `Brainstem::attempt_growth()` sucht schalenweise nach
dem nächsten räumlichen Radius, verhindert Selbst- und Doppelverbindungen und
legt eine erregende gerichtete Synapse mit Gewicht null an. Der getrennte
Growth-Zufallszustand wird persistiert und ist Teil des Fingerabdrucks.

## Vollständige Zugänglichkeit

Die exhaustive Exploration wählte bei 32, 128, 256, 512 und 2.048 Neuronen
jedes nicht reservierte Neuron genau einmal aus. Es gibt keinen Ausschluss nach
ID, Position, bisheriger Aktivität oder Verbindungsgrad. Die acht in diesem
Versuch ausgewiesenen sensorischen Schnittstellen bleiben von der internen
Exploration getrennt.

Ein explorativ aktiviertes, unverbundenes Neuron wächst garantiert als Sender
in die Struktur. Sein Ziel erhält dadurch einen Eingang; der Sender besitzt
einen Ausgang. Damit ist das Henne-Ei-Problem technisch durchbrochen, ohne den
späteren Nutzen oder ein richtiges Motorziel vorzugeben.

## Nullgewicht, Eligibility und Kausalität

Nullgewicht-Impulse werden als lokale Ereignisse erfasst. Wenn das Ziel durch
eine unabhängige lokale Koinzidenz feuert, kann die neue Synapse in eine
`active_phase`-Eligibility gelangen. G3 und ternäre Modulation verändern sie
anschließend über den unveränderten Drei-Faktor-Pfad von 0 auf 1 und 2.

Der kontrollierte Nachweis gelang in 768/768 Kombinationen. Bei Gewicht 2 ließ
der neue Weg sein Ziel allein feuern. W1 setzte ausschließlich diese Synapse
auf null und entfernte den Effekt in 768/768 Fällen; W2 reproduzierte die
unveränderte Fortsetzung exakt. Growth-off, Exploration-off und Write-off
blieben wirkungslos. Eine statisch bereitgestellte Nullgewicht-Verbindung
konnte unter derselben Erfahrung ebenfalls lernen; nicht das Erzeugen allein,
sondern lokale Koinzidenz plus Modulation ist der funktionale Engpass.

## Ursache des B.4.4-Befunds

B.4.4 übertrug zwar tausende Impulse über neue Verbindungen, erzeugte aber
keine geeignete Kombination aus Zielneuronfeuer, Auswahl der neuen Synapse in
Eligibility und nachfolgender positiver Modulation. Der Engpass ist mit den
heutigen Mechanismen technisch überwindbar, aber nicht automatisch behoben:
Die kontrollierte Koinzidenz beweist Möglichkeit, nicht eine bereits
vorhandene autonome Rekrutierungsstrategie.

## Architekturgrenzen

Growth kennt die Reservierungen der Exploration nicht. In der vollständigen
Matrix zielten 221 von 3.840 ersten Wachstumsereignissen auf eine der acht
geschützten Schnittstellen. Zudem erzeugt Growth ausschließlich erregende
Verbindungen. Eine Produktionsaktivierung könnte daher angeborene Bereiche
strukturell verändern und besitzt noch keine lokale Regel, die nützliche
Koinzidenz autonom wahrscheinlicher macht.

Die Forschungsimplementierung verändert deshalb keine Produktionsregel. In
den historischen Aufgaben wird Growth nur für einen begrenzten internen
Explorationspuls geöffnet und vor der nächsten Motoraufgabe wieder geschlossen.
Geschützte Neuronen werden nicht explorativ als Sender gewählt; Growth kann sie
aber weiterhin als Ziel treffen. Genau diese verbleibende Konfundierung wird
in der schutzbereinigten Teilpopulation getrennt ausgewiesen.

## Persistenz und Aufwand

Neu entstandene Synapsen, Membranstatus, Ereigniswarteschlange und Growth-RNG
überstehen Speichern und Laden identisch. Die Fortsetzung bleibt
fingerprint-identisch. Es entstehen keine neuen persistenten Felder je Neuron
oder Synapse. Der 2.048-Neuronen-Test bleibt gültig und begrenzt.
