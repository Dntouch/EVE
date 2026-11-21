# Forschungsbericht – Forschungsstufe B.5.10

## Ergebnis

B.5.10 isoliert erstmals einzelne positive synaptische Schreibereignisse am
tatsächlichen Modulationskontakt. Die technische Kausalfrage ist gelöst: Eine
einzelne Operation kann ausgelassen werden, ohne den normalen Modulationsweg,
andere Kandidaten oder den Eligibility-Verbrauch absichtlich zu verändern.

Wissenschaftlich bleibt die lokale Auswahlfrage offen. Historisch sind 13
konkrete Schritte auf zehn Problemverbindungen kausal. Prospektiv sind jedoch
notwendige, neutrale und verzögert schädliche positive Änderungen Teil
desselben natürlichen Lernwegs. Die zusätzlich extern rekonstruierte Historie
generalisiert schlechter als die aktuelle lokale Signatur. Eine neue
Produktionsregel wird deshalb nicht freigegeben.

## Reproduktion und Umfang

Die Referenzen wurden exakt reproduziert:

| Referenz | Robust |
|---|---:|
| B.5.7 Discovery | 242/384 |
| B.5.7 Bestätigung | 254/384 |
| B.5.8/B.5.9 Discovery | 240/384 |
| B.5.8/B.5.9 Bestätigung | 244/384 |

L0–L4 reproduzieren 49,49 %, 51,19 %, 53,46 %, 53,51 % und 53,51 %
Bestätigungsgenauigkeit. Die Stabilitäts- und Umkehrkontrollen aus B.5.8
bleiben erhalten. Die neuen vollständigen Populationen umfassen je 384 Netze
mit Seeds 70001–70064 und 80001–80064; alle Netze waren gültig. Sie erreichten
223/384 und 260/384 robuste Fälle und dienen nicht der Regeloptimierung.

Die Zeitkontrollen ergaben für Fest-1, Fest-12, Fest-32 und unregelmäßige
Abstände 0/384, 0/384, 268/384 und 164/384 robuste Netze. No-Learning blieb
bei 0/384. S4-d16 und die globale Schreibsperre bleiben experimentell und
standardmäßig deaktiviert.

## Zentrale Befunde

### Historische Kausalität

Die zwölf bekannten Verlustfälle enthalten erneut 52 erhöhte Gewichte und
zehn einzeln rücknehmbare Problemverbindungen. Auf ihnen wurden 13 konkrete
positive Schreibereignisse identifiziert. Das isolierte Auslassen jedes
einzelnen Schritts erhält die Doppelbeziehung bis zum jeweiligen historischen
Verlustzeitpunkt. Die kombinierte Unterdrückung der später erhöhten
Verbindungen wirkt in 12/12 Fällen.

### Prospektive Kausalität

119 Ereignisse wurden ohne Kenntnis ihrer Zukunft ausgewählt, davon 77
erregende und 42 inhibitorische. 21 waren unmittelbar notwendig; 98 änderten
die festgelegte Funktion zunächst nicht. Von diesen 98 wurden bis Erfahrung
128 drei Änderungen verzögert nützlich und elf verzögert schädlich; 84 blieben
im Fenster neutral. Elf notwendige Ereignisse lagen im Erwerb und zehn in der
Umkehr.

Die Stichprobe umfasst 42 positive inhibitorische Änderungen. Die historische
B.5.9-Population enthält zwei inhibitorische Problemverbindungen. Damit ist
inhibitorische Verstärkung weder pauschal harmlos noch allein aus ihrer
Wirkungsart als schädlich erkennbar.

### Lokale Geschichte

L0 klassifizierte 53/59 Bestätigungsereignisse, L4 51/59. Das ist bei nur 59
Bestätigungsereignissen, einer neutral dominierten Zielverteilung und einer
vorselektierten Ereignisstichprobe kein Freigabenachweis. H1–H4 fielen wegen
unbekannter Signaturen auf 34/59, 28/59, 22/59 und 22/59. Zudem sind diese
vollständigen Historien nur extern rekonstruiert und daher keine zulässige
Produktionsinformation.

## Antworten A–R

**A. Welches einzelne Ereignis verursacht die historischen Verluste?** Auf
den zehn Problemverbindungen wurden 13 konkrete Schritte gefunden. Jeder
Schritt ist im beobachteten natürlichen Pfad kausal notwendig: Sein isoliertes
Auslassen erhält beide Beziehungen bis zum historischen Verlustzeitpunkt.

**B. Welche Fälle benötigen mehrere Verstärkungen?** Zwei Verbindungen wurden
zweimal erhöht. Beide Einzelschritte sind jeweils notwendig; die zweite
Erhöhung allein erklärt den Verlauf daher nicht. Auf Fallebene stellt die
kombinierte Unterdrückung der erhöhten Verbindungen 12/12 Fälle wieder her.

**C. Kann eine Verstärkung erst nützlich und später schädlich sein?** Diese
Abfolge wurde nicht nachgewiesen. Elf prospektive Änderungen waren zunächst
neutral und später schädlich; keine war zunächst messbar nützlich und später
schädlich.

**D. Was war lokal vorhanden?** Gewicht, Wirkung, Empfängerzustand,
Aktivitätszähler, Eligibility und Modulationspolarität waren direkt vorhanden;
Marge und aktuelle Konkurrenz waren lokal berechenbar. Vollständige Historie
und spätere Wirkung waren es nicht.

**E. Trennt die Beitragsgeschichte notwendige und schädliche Änderungen?**
Nicht belastbar. Die extern protokollierte Geschichte erhöhte die Zahl
unbekannter Signaturen und verschlechterte die unabhängige Klassifikation.

**F. Welche Bedeutung hat die Margenentwicklung?** Die aktuelle Marge bleibt
diagnostisch nützlich. Eine extern rekonstruierte Margenfolge lieferte in H2
keinen Zusatzgewinn gegenüber H1 und ist nicht lokal gespeichert.

**G. Welche Bedeutung hat wiederholte Eligibility?** Sie beschreibt lokale
Beteiligung, trennte die kausalen Wirkungsklassen in dieser Stichprobe aber
nicht sicher. Eine vollständige Eligibility-Geschichte existiert nicht als
neuronaler Zustand.

**H. Welche Rolle spielt wechselnde Konkurrenz?** Aktuelle Konkurrenz ist Teil
von L4. Die externe Konkurrenzgeschichte H3 verbesserte H2 nicht. Sie erklärt
Einzelfälle, liefert aber keinen generalisierenden Selektor.

**I. Können inhibitorische Erhöhungen schädlich sein?** Ja. Zwei der zehn
historischen Problemverbindungen sind inhibitorisch. Die 42 neuen
prospektiven inhibitorischen Ereignisse zeigen, dass der Effekt allein keine
Wirkungsklasse festlegt.

**J. Wie oft ist eine positive Änderung zunächst neutral?** 98/119 oder
82,35 % der prospektiv ausgewählten Ereignisse.

**K. Wie viele neutrale Änderungen werden später wirksam?** 14/98: drei wurden
nützlich, elf wurden schädlich; 84 blieben bis 128 Erfahrungen neutral.

**L. Welche notwendigen Änderungen würde Stabilisierung verlieren?** Mindestens
die 21 beobachteten unmittelbar notwendigen Ereignisse: elf im Erwerb und zehn
in der Umweltumkehr. Eine pauschale Sperre ist damit unzulässig.

**M. Kann lokale Information prospektiv unterscheiden?** Sie zeigt ein
Signal, aber noch keinen belastbaren Selektor. L0 erreicht 53/59 in der kleinen
Bestätigung; Stichprobenumfang, Klassenverteilung und fehlende aktive
Regelgegenprobe reichen nicht zur Freigabe.

**N. Was benötigt externes oder zukünftiges Wissen?** Vollständige Beitrags-,
Margen-, Konkurrenz- und Schreibfolgen sind Klasse C. Erwerbs-, Retentions-
oder Schadenswirkung ist Klasse D. Beides darf keine Produktionsregel steuern.

**O. Kann Nichtändern nützlich sein, ohne Umlernen zu beschädigen?** Im
Einzelfall ja: W1 verhindert historische und elf verzögerte prospektive
Schäden. Eine allgemeine Auswahl ist nicht belegt; dieselbe pauschale
Nichtänderung würde 21 notwendige Schritte verlieren.

**P. Reichen die vorhandenen Mechanismen aus?** Für exakte Beobachtung und
Intervention ja. Für eine sichere lokale Entscheidung über Schreiben oder
Nichtschreiben ist der Nachweis noch nicht erbracht.

**Q. Ist eine neue Lernfunktion gerechtfertigt?** Nein. K23/K24 werden nicht
implementiert. Es fehlen eine präregistrierte A/B-Regel, aktive kausale
Validierung und ausreichende unabhängige Fallzahl.

**R. Was folgt für B.5.11?** Eine ausschließlich A/B-basierte, vorab
festgelegte Selektionshypothese muss auf einer größeren kausalen
Ereignisstichprobe geprüft werden. Erst danach wäre eine standardmäßig
deaktivierte Forschungsvariante vertretbar.

## Grenzen

- Nur 119 detaillierte neue Ereignisse wurden fortgeführt; die vollständigen
  Populationen dienen der unveränderten Referenzklassifikation.
- Ein Ereignis pro präregistriertem Detailverlauf verhindert eine verdeckte
  Bestwertsuche, begrenzt aber seltene Klassen.
- Die Zielverteilung ist stark neutral; Genauigkeit allein ist daher keine
  Regelfreigabe.
- H1–H4 sind in der geprüften Form Forschungsprotokolle, keine lokale
  Gedächtnisstruktur.
- Das Beobachtungsfenster endet nach 128 natürlichen Folgeerfahrungen.

## Validierung und Merge-Empfehlung

726/726 Kerntests bestehen. W0 und W2 waren in 119/119 Fällen identisch, W1
unterdrückte 119/119-mal exakt eine Operation, alle Zustände blieben gültig.
Persistenz und deterministische Fortsetzung sind unverändert; Formatversion 15
bleibt bestehen. Es entstehen keine persistenten Bytes je Neuron oder Synapse.

**B.5.10 nach `main` mergen: Ja.**

Die Empfehlung gilt für Diagnostik, Tests, Runner, Rohdaten und Dokumentation.
Sie ist ausdrücklich keine Freigabe einer neuen neuronalen Produktionsregel.
