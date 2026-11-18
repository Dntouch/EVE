# Technische Analyse – lokale Entscheidungsbindung in B.4.5

## Verarbeitung einer Entscheidung

Ein Reiz feuert seinen sensorischen Kanal. Gewichtete Impulse können A, B,
beide oder keinen Motor auslösen. Beim postsynaptischen Feuer erzeugt
`active_phase` aus den seit der letzten lokalen Aktivitätsgrenze tatsächlich
aktiven Eingängen eine Eligibility-Entscheidung; Nullgewicht-Synapsen bleiben
ausdrücklich zulässig. Die Rückmeldung erreicht A und B über denselben
allgemeinen neuronalen Modulationsweg und kennt keine richtige Handlung.

Bei einer eindeutigen Entscheidung verstärkt positive beziehungsweise
schwächt negative Modulation die lokalen Kandidaten und verbraucht die Spur.
Bei doppelter Motoraktivierung setzt der bisherige Runner die Polarität auf
neutral. Neutrale Modulation ändert keine Gewichte und verbraucht die Spur
nicht. „Keine Modulation“ erzeugt zusätzlich nicht einmal die neuronalen
Modulationsereignisse, ist aber verhaltensgleich.

## Diagnostischer Befund

In K1 entstehen im gemischten Training durchschnittlich 70,22 doppelte
Ausgaben pro 128 Episoden; in der zweiten Hälfte 41,20 gegenüber 29,02 in der
ersten. Doppelaktivierung nimmt also trotz Training zu. Bei diesen Ereignissen
liegt der mittlere Gewichtswert des zum aktuellen Reiz richtigen Pfads bereits
bei 2,94, der konkurrierende Pfad aber ebenfalls bei 1,75. Beide Motorwege
sind funktional, sodass neutrale Rückmeldung keine weitere Trennung bewirkt.

Keine der 8.192 K1-Modulationen traf eine Eligibility-Entscheidung aus einer
früheren Episode. Variation von Abstand, Dauer und Feedbackzeitpunkt änderte
den besten Mischwert nicht. H1 ist damit nicht als dominanter Fehler belegt.

K4 mit Kandidatenkapazität 1 fällt exakt auf No-Learning zurück. Kapazitäten 2,
4 und 8 erreichen 34,63 %, 35,85 % und 35,85 %. Die vorhandene lokale
Eingangsabgrenzung ist also technisch möglich und reduziert Fremdreizanteile,
reicht aber nicht gegen die motorische Konkurrenz.

## Kausale Ambiguitätsintervention

Negative Modulation ausschließlich bei Doppelaktivierung ist ein gezielter
Eingriff in den vermuteten Engpass. Mit zunehmender Eingriffsrate steigen die
Mischwerte von 53,28 % (25 %) über 65,55 % (50 %) und 72,82 % (75 %) auf
76,82 % (100 %). Bei 100 % sinken doppelte Trainingsausgaben von 70,22 auf
25,48. Gegen K1 beträgt der gepaarte Vorteil 40,97 Punkte, 95-%-Intervall
[36,46; 45,46]; Doppelhandlungen sinken bei allen 64 Seeds.

Der Eingriff ist trotzdem keine Lösung: Er schwächt beide aktiven Motorpfade,
erreicht im Entdeckungssatz nur 9/64 robuste Seeds und in unabhängigen Seeds
14/64. Die bestätigten Mittelwerte 81,44 % für X und 73,88 % für Y verfehlen
eine Einzelschwelle. Pauschale negative Ambiguitätsrückmeldung ersetzt daher
keine lokale Konkurrenzauflösung.

## Entscheidung zu C1/C2

C1 wird nicht implementiert: K3 und K4 liefern keinen Nachweis, dass eine neue
zeitliche/Koinzidenzbindung den beobachteten Fehler behebt. H2 ist dagegen
kausal gestützt. Eine C2-Hemmung wäre jedoch ein neuer neuronaler Mechanismus;
ohne zuvor spezifizierte lokale inhibitorische Semantik würde eine sofortige
„erste Ausgabe gewinnt“-Regel lediglich Ereignisreihenfolge als technische
Handlungsauswahl verkleiden. C2 wird deshalb für B.4.6 vorbereitet, nicht in
B.4.5 als Erfolg simuliert.
