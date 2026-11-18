# Forschungsbericht – Forschungsstufe B.4.1

## A. Ausgangssituation

B.4 hatte lokale Gewichtsänderungen, aber keinen Verhaltensvorteil gezeigt.
Der Validierungsauftrag benannte fehlenden Zeitfortschritt, gekoppelte
Zufallsströme, eine unzureichende Explorationskontrolle, eine isolierte
Kleinschaltung und eine ungeklärte motorische Wirkung der Gewichte.

## B. Reproduktionsprüfung

Der historische Commit `87c2cd0` wurde separat gebaut und erneut ausgeführt.
`raw_trials.csv`, `resources.csv` und `summary.csv` stimmen bytegenau mit B.4
überein. Die SHA-256-Werte sind in `audit-reproduction.csv` dokumentiert. Die
vorhandenen B.4-Daten wurden nicht verändert.

## C. Code-Audit

Alle fünf vorgegebenen Einschränkungen wurden bestätigt. Reguläre B.4-Episoden
blieben bei derselben Simulationszeit; nur ausdrücklich verzögerte Rückmeldung
bewegte sie. Aufgaben und zufällige Bewertung teilten einen RNG. K12
unterdrückte zugleich den Sensorreiz. Motorneuronen waren Ziele der
Bewertungsroute.

Der Korrekturlauf zeigte zusätzlich, dass gewöhnlich verarbeitete
Modulationskontakte in G3 gelangen und später selbst verstärkt werden konnten.
Sie werden nun als nicht-erregende lokale Rezeptorkontakte verarbeitet: Das
Modulationsereignis erreicht das Neuron, verändert aber weder Membran noch G3
und kann keinen Motorausgang auslösen.

## D. Simulierte Zeit

Der korrigierte Runner weist jeder Episode einen Startpunkt zu und verwendet
ausschließlich `advance_time_to` beziehungsweise `advance_time_by`. Bei Abstand
16 beginnen die Episoden bei 0, 16, 32 und so weiter. Fällige Abläufe und
R2F-Erholungen werden verarbeitet, zukünftige bleiben in der Warteschlange.

K16 ohne Zeitfortschritt erzeugte keine natürlichen Abläufe, nur 124 positive
und 78 negative Änderungen und endete bei −1. K17 mit Zeitfortschritt erzeugte
3.148 positive sowie 3.554 negative Änderungen, blieb mit −0,855957 aber
ebenfalls unter K0. Die alte zeitlose Versuchsanordnung war daher methodisch
falsch, verbarg in dieser Aufgabe jedoch keinen positiven Lernerfolg.

## E. Zufallsquellen

Topologie, Aufgaben, Exploration und zufällige Rückmeldung besitzen getrennte,
dokumentierte Seeds. Alle 30 Varianten eines Seeds lesen denselben vorab
erzeugten Plan. Zusätzliche Bewertungsziehungen können die spätere Aufgabe
nicht verändern. Die 12.288 Planzeilen stehen in `tasks.csv`.

## F. Neuronale Zwei-Handlungs-Umgebung

Sensoren 0 und 1, explorative Verarbeitungsneuronen 2 bis 5, Motor A und B sowie
die Bewertungsinputs sind getrennt. Modulation erreicht A und B über eigene
gewichtlose Rezeptorkontakte. In der Vorprüfung waren A und B bei allen 64
Seeds erreichbar – sowohl im 64-Neuronen-Netz als auch in der 256-Neuronen-
Konfiguration mit 1.024 regulären Startsynapsen, V2 und R2F.

## G. Exploration

X0 bis X3 behalten denselben Sensorreiz und variieren nur Zusatzaktivität.
Ohne Exploration blieb die Belohnung −1. Geringe, mittlere und starke
Exploration hielten Alternativen zunächst erreichbar, wurden im Training aber
durch negative Änderungen zunehmend abgeschwächt. X1 endete bei −0,807617,
X2 bei −0,855957 und X3 bei −0,900879.

## H. Lernspuren

Unter identischer Umwelt und korrekter Zeit ergaben sich:

| Variante | Auswertungsbelohnung | Delta gegen K0 | Null→wirksam |
|---|---:|---:|---:|
| K0 | −0,397949 | 0 | 0 |
| E1 | −0,855957 | −0,458008 | 785 |
| E2 | −0,909180 | −0,511230 | 0 |
| E3 | −0,977539 | −0,579590 | 1.032 |

Das 95-%-Bootstrapintervall von E2 gegen K0 beträgt [−0,550781;
−0,472168]. E2 war damit belastbar schlechter, nicht besser. E1 ist breiter,
E2 markiert nur den feuerentscheidenden Eingang, E3 markiert zu viele bloße
Membranbeteiligungen.

## I. Nullgewicht-Synapsen

E2 aktivierte weiterhin keine Nullgewicht-Verbindung, weil ein Eingang ohne
Membranbeitrag nicht die feuerentscheidende Synapse sein kann. E1 kann den
vorherigen Nullgewicht-Sensorinput über G3 aufnehmen; E3 markiert ihn direkt.
Die Kapazität, Zeit und Modulationsroute waren in den erfolgreichen
Einzelversuchen nicht begrenzend.

## J. Gewichtsänderung und Handlungswirkung

Im kontrollierten Kausalversuch wurde eine Sensor-zu-A-Synapse zweimal aus
eigener sensorischer und explorativer Aktivität positiv moduliert. Vorher löste
der Sensor A in 0/64 Seeds aus, danach in 64/64. Eine diagnostische Kopie mit
dem Gewicht wieder auf null löste A in 0/64 aus. Damit ist der kausale Einfluss
einer lokalen Gewichtsänderung auf eine spätere Handlung nachgewiesen – nicht
aber erfolgreiches freies Lernen.

## K. Hauptlernexperiment

Nach 128 Trainings- und 64 Auswertungsepisoden ohne neue Plastizität blieb K0
bei −0,397949. Korrekte E1-Rückmeldung endete bei −0,855957, zufällige bei
−0,889648 und vertauschte bei −0,880859. E2 unterschied sich nicht belastbar
von zufälliger oder vertauschter Modulation, lag aber eindeutig unter K0.
Fehlende und doppelte Handlungen sind vollständig enthalten; kein Seed wurde
entfernt.

## L. Erweiterte Lernverläufe

K0 blieb nach 512 Erfahrungen bei −0,359375 und nach 2.048 bei −0,405762.
E1, E2, E3 sowie zufällige und vertauschte Rückmeldung endeten bei beiden
Längen jeweils bei −1. Mehr Erfahrung stabilisierte somit das Scheitern, nicht
die Fähigkeit.

## M. Umweltumkehr

Die Umweltregel wurde nach Episode 64 vertauscht, ohne neuronale Zielvorgabe.
Die mittlere Belohnung sank von −0,489746 vor der Umkehr auf −0,773438 im
zweiten Trainingsabschnitt und −0,847656 in der Auswertung. Unterdrückte Wege
wurden nicht zuverlässig wieder erreichbar.

## N. Vergleich 64 gegen 256 Neuronen

K20 erreichte bei 256 Neuronen −0,851562 gegenüber −0,855957 in der isolierten
E1-Schaltung. Beide Motorwege waren strukturell und funktional erreichbar. Das
negative Ergebnis ist daher nicht allein eine Folge fehlender Erreichbarkeit
im kleinen Netz; die größere reguläre Topologie löst das Lernproblem nicht.

## O. Ressourcen und Stabilität

Gemessen wurden 45.712 Byte und 23 Synapsen bei 64 Neuronen sowie 222.640 Byte
und 1.073 Synapsen bei 256 Neuronen. Das entspricht 714,25 beziehungsweise
869,688 Byte pro Neuron einschließlich fester Laufzeitkosten. Die Projektionen
für 10.000 bis eine Million Neuronen sind ausdrücklich nur lineare
Orientierungswerte. Gewichte blieben in [0,3], Budgets und feste
Spurkapazitäten wurden eingehalten; technische Abbrüche traten nicht auf.
Persistenzformat 10 erhält neben ausstehenden Ereignissen auch die komplette
Rückmeldungsroute; die Tests setzen nach dem Laden mit neuen Handlungen und
neuer Modulation deterministisch fort.

## P. Abschlussbewertung

Die methodischen Fehler von B.4 sind bestätigt und korrigiert. Die Lernregel
kann Nullgewicht-Verbindungen wirksam machen, und solche Änderungen können
eine spätere Handlung kausal verändern. Tatsächliches erfahrungsabhängiges
Lernen im freien Hauptversuch wurde dennoch nicht nachgewiesen.

Der konkrete Engpass ist die Wechselwirkung aus breiter lokaler Zuordnung und
negativer Modulation: Bereits genutzte Explorationswege werden abgeschwächt,
bevor reizabhängige Alternativen stabil genug entstehen. E2 schützt zwar vor
breiter Zuordnung, kann aber Nullgewicht-Sensorpfade prinzipbedingt nicht
aufbauen. Eine neue Lernregel ist daraus noch nicht abzuleiten; zuerst muss der
Erhalt alternativer Exploration isoliert geprüft werden.

Alle 260 automatisierten Tests bestehen. Rohdaten, Zeitabläufe,
Gewichtsänderungen, gepaarte Vergleiche, Kausalproben und Erreichbarkeit liegen
unter `research/results/b41/`.
