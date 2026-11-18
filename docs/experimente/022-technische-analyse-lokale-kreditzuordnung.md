# Technische Analyse – lokale Kreditzuordnung in B.5.0

## Methodische Basis

B.5.0 verwendet unverändert das 32-Neuronen-System aus B.4.9. Der
B.4.9-Runner wird eingebettet; Netzstruktur, Schwelle 2, Gewichtsgrenze 3,
G3, `active_phase`-Eligibility, ternäre Modulation, geschützte Exploration und
lernbare Hemmung bleiben unverändert. Wachstum und globale Handlungswahl sind
deaktiviert. Es entsteht kein neuer neuronaler Zustand.

K0–K3 sowie die drei B.4.8-Explorationskontrollen werden über drei
Stromfamilien, normale und inverse Zuordnung und 64 Seeds reproduziert. E1 und
E2 starten weiterhin aus getrennten, fingerprint-identischen Kopien. 384/384
doppelt ausgeführte K0-Läufe stimmen in Fingerabdruck, E1, E2, Gewichten und
Netzgültigkeit exakt überein.

## Prospektive Stichprobe

Die Untersuchungspopulation umfasst vorab festgelegt alle 49.152 Episoden aus
384 K0-Netzen. Sie enthält:

- 21.069 bereits autonom richtige Episoden,
- 13.727 erstmalig durch Exploration richtige Episoden nach autonomer Stille,
- 13.761 erfolglose Explorationsereignisse,
- 583 autonome Fehlschläge ohne Exploration und
- 12 mehrdeutige Ausgaben.

Für die kausale P1-Diagnose wird je Familie-Zuordnung-Seed-Cluster das erste
zulässige positive Explorationsereignis verwendet. Damit entstehen 384
unabhängig ausgewählte Zustandskopien. Die Ereignisse liegen in Episode 0 bis
8, im Mittel bei 1,01. Diese frühe Stichprobe ist nicht auf späteren Erfolg
selektiert. Eine getrennte P4-Stichprobe wählt je Cluster das erste positive
Explorationsereignis, vor dem die andere Reizbeziehung bereits autonom korrekt
war; sie liegt im Mittel bei Episode 9,08.

## Ereignis- und Eligibility-Kette

Jede Diagnose startet von der vollständigen Runtime unmittelbar vor dem
natürlichen Trainingsereignis. Feuer- und Impulsbeobachter trennen Sensor- und
Explorationsphase. Pro Eligibility-Eintrag werden Empfänger, Generation,
Ablaufzeit, Stärke, Kandidat, Sender, Wirkung, Gewicht, Phasenbeteiligung und
tatsächliche Änderung gespeichert.

Die 384 ersten positiven Ereignisse enthalten 1.245 inventarisierte
Kandidaten:

| lokale Rolle | Kandidaten | geändert | funktional mit Gewicht > 0 |
|---|---:|---:|---:|
| Ziel-Reizsynapse | 378 | 378 | 0 |
| Ziel-Explorationssynapse | 384 | 0 | 384 |
| Ziel-Ausgangssynapse | 384 | 0 | 384 |
| konkurrierende Hemmsynapse | 99 | 99 | 0 |

Die geschützten Explorations- und Ausgangssynapsen tragen die erfolgreiche
Aktivität kausal, ändern ihr Gewicht aber nicht. Die sensorische
Nullgewicht-Synapse wird durch die lokale Ereignisgeschichte korrekt in die
Eligibility aufgenommen und in 378 Fällen von 0 auf 1 erhöht. Zusätzlich
steigt in 99 Fällen eine Hemmsynapse von 0 auf 1, obwohl sie bei dieser
erfolgreichen Handlung keinen positiven Eingangsbeitrag lieferte.

Damit ist H2 in seiner starken Form widerlegt: Die Verbindung zwischen Reiz
und explorativ aktiviertem Entscheidungsweg fehlt nicht. Sie wird lokal
gebildet. H1 ist teilweise bestätigt, weil zugleich nichtursächliche lokale
Kandidaten modifiziert werden.

## Kausale Zustandskopien

Alle P1-Kopien bleiben nach der ersten positiven Erfahrung beim nächsten Reiz
ohne autonome Antwort. Dasselbe gilt ohne Exploration, ohne Plastizität,
unter neutraler Modulation, bei isolierten Sensoränderungen und bei isolierten
konkurrierenden Änderungen. Die Ziel-Reizsynapse besitzt danach Gewicht 1 und
bleibt unter der Feuergrenze 2. Der fehlende erste Abruf ist damit keine
fehlende Kreditzuordnung, sondern unzureichende synaptische Wirksamkeit.

Verzögerungen von 0, 4 und 8 Zeiteinheiten liegen noch im Eligibility-Fenster,
führen aber weiterhin nur zur subthreshold Änderung. Bei 16 und 32 Einheiten
sind die relevanten Spuren abgelaufen. Keine Timingvariante erzeugt nach einer
einzigen Erfahrung einen autonomen Abruf.

## Wiederholte positive Erfahrung

Die Wiederholungsdiagnose führt dieselbe tatsächlich erfolgreiche
Reiz-Explorations-Kette erneut aus. Sie ist eine Zustandskopie-Diagnose und
wird nicht als natürliche Erfolgsquote ausgegeben.

Nach der ersten Erfahrung sind 0/384 Kopien autonom korrekt. Nach der zweiten
sind es 364/384 (94,79 %, clusterbewusstes 95-%-Intervall 92,45–96,88 %), nach
der dritten und vierten 384/384. In 364 Kopien erhöht die zweite positive
Modulation ausschließlich den relevanten Zielpfad von 1 auf 2. Die gezielte
Rücknahme auf 1 beseitigt den Abruf in allen 364 Kopien; die isolierte
Übernahme des Gewichts 2 stellt ihn in allen 364 wieder her. Diese Änderung
ist damit im untersuchten Zustand notwendig und hinreichend.

Sechs weitere zweite Änderungen gehen erst von 0 auf 1 und erzeugen noch
keinen Abruf. Die restlichen Fälle erreichen den Schwellenzustand mit der
dritten Erfahrung. Eine einzelne Schrittweite von +1 kann eine
Nullgewicht-Verbindung bei Schwelle 2 prinzipbedingt nicht sofort autonom
wirksam machen.

## Interferenz

In den 384 P4-Kopien ist die andere Beziehung vor dem neuen Ereignis korrekt.
Nach einer positiven Explorationshandlung wird die neue Beziehung unmittelbar
in 22/384 Fällen korrekt, die alte bleibt in 376/384 Fällen erhalten; beide
sind in 22/384 Fällen gleichzeitig korrekt. Die neue Ziel-Reizsynapse ist in
diesen späteren Zuständen nicht immer null, erreicht aber nur selten eine
eindeutig wirksame Konfiguration.

Werden ausschließlich die sensorischen Änderungen behalten, steigt der neue
Abruf auf 40/384. Zusätzliche Hemmungs- und andere lokale Änderungen sind also
nicht für den neuen Erfolg erforderlich und können ihn in einzelnen Kopien
behindern. Die Rücknahme der Ziel-Reizänderung beseitigt alle 22 D0-Erfolge.
Die alte Beziehung bleibt dennoch in 97,92 % der Fälle erhalten; unmittelbare
Zerstörung der zweiten Beziehung ist nicht der primäre P4-Fehler.

Nach 16 identischen zukünftigen Erfahrungen erreicht die D0-Fortsetzung im
Mittel 3,63 Prozentpunkte weniger E1 als die plastizitätsfreie
Aktivitätskontrolle. Eine einzelne positive Erfahrung garantiert daher weder
langfristige Stabilität noch einen Vorteil im gemischten Strom.

## Entscheidung gegen K9

Die Kandidateninventur und die Einzelgewichtsablation identifizieren keinen
fehlenden Mechanismus zur lokalen Zuordnung des Reizes. Die vorhandene Regel
nimmt genau die anfänglich inaktive Ziel-Reizsynapse auf; wiederholte positive
Erfahrung erhöht sie bis zur Schwelle, und der zweite Schritt ist kausal
notwendig und hinreichend.

Eine neue Kandidatenregel würde den isolierten Hauptfehler der ersten
Erfahrung – Gewicht 1 bei Schwelle 2 – nicht lösen. Zugleich ist noch nicht
geklärt, wie natürliche Wiederholungen, zusätzliche Hemmungsänderungen und
gemischte spätere Erfahrungen zusammenwirken. K9 und K10 werden deshalb nicht
implementiert.

## Ressourcen und Regression

Alle Zustände bleiben gültig, Wachstum bleibt null, die Zustandsschätzung
liegt bei rund 24,5 KiB und die Queue bleibt begrenzt. 64/64
Persistenzkontrollen werden exakt restauriert und deterministisch fortgesetzt.
Die bisherigen 473 Tests und 18 neue B.5.0-Prüfungen bestehen: 491/491.

