# Entscheidung 0022: Bestehende lokale Kreditzuordnung ist grundsätzlich ausreichend

- Status: angenommen
- Datum: 18. November 2026
- Bezug: Entscheidung 0021

## Kontext

B.5.0 erfasst prospektiv 49.152 Trainingsereignisse aus 384 unabhängigen
Familie-Zuordnung-Seed-Clustern. Je Cluster wird das erste Ereignis kopiert,
bei dem ein Reiz keine autonome Antwort erzeugt, geschützte Exploration eine
eindeutige richtige Handlung aktiviert und erst diese tatsächlich erfolgreiche
Handlung positiv bewertet wird.

Die sensorische Nullgewicht-Synapse des erfolgreichen Weges erhält in 378/384
Fällen lokale Eligibility und steigt auf Gewicht 1. Der Kredit erreicht damit
die richtige Reiz-Handlungs-Verbindung. Gewicht 1 bleibt jedoch unter der
Feuergrenze 2, sodass nach einer Erfahrung 0/384 autonome Abrufe entstehen.

Nach einer zweiten real erfolgreichen Wiederholung erreichen 364/384 Kopien
den autonomen Abruf. Die Änderung des Zielpfads von 1 auf 2 ist in allen 364
passenden Einzelablationen notwendig und hinreichend. Nach einer dritten
Wiederholung sind 384/384 Kopien korrekt.

Positive Modulation verändert zusätzlich 99 nichtursächliche Hemmspuren. In
der P4-Interferenzprobe bleibt eine vorhandene zweite Beziehung zwar in
376/384 Fällen unmittelbar erhalten, doch zusätzliche Nicht-Sensoränderungen
verringern den neuen Abruf gegenüber der isolierten Sensoränderung. Auch die
spätere gemischte Fortsetzung zeigt keinen stabilen allgemeinen Vorteil.

## Entscheidung

Eine neue lokale Kreditzuordnung K9 wird nicht implementiert oder freigegeben;
K10 entfällt. Der untersuchte Hauptfehler ist keine fehlende Verbindung von
Reiz und Exploration. Die bestehende Eligibility-Regel kann den richtigen
Sensorpfad auswählen und durch wiederholte positive Erfahrung bis zur
autonomen Wirksamkeit verstärken.

Als gesichert gelten:

- erstmalige positive Explorationshandlungen müssen von bereits autonomen
  Erfolgen getrennt werden,
- die lokale Ereignisgeschichte überbrückt Sensor- und Explorationsphase,
- eine einzelne Verstärkung von 0 auf 1 bleibt bei Schwelle 2 unwirksam,
- der zweite Schritt von 1 auf 2 kann notwendig und hinreichend sein,
- positive Modulation erreicht teilweise nichtursächliche lokale Kandidaten,
- der isolierte Erwerb beweist noch keine Stabilität in gemischter Erfahrung,
- Persistenz und getrennte E1-/E2-Auswertung bleiben technisch exakt.

Die allgemeine Lernfreigabe bleibt gesperrt. Diagnostisch isolierte
Sensoränderungen sind keine zulässige Produktionslernregel.

## Auftrag für B.5.1

B.5.1 soll untersuchen, unter welchen natürlichen Erfahrungsfolgen derselbe
Reiz wiederholt erfolgreich exploriert wird, den notwendigen Schwellenübergang
erreicht und anschließend autonom abrufbar bleibt. Dabei sind insbesondere
zu trennen:

- Häufigkeit und zeitlicher Abstand wiederholter positiver Erfahrungen,
- Verlust bereits aufgebauter Gewichte durch negative oder konkurrierende
  Episoden,
- Beitrag zusätzlicher Hemmungs- und Nicht-Sensoränderungen,
- Stabilität beider Reizbeziehungen nach dem ersten autonomen Abruf,
- normale und inverse Rollen über unabhängige Stromfamilien.

Lernschritt, Feuergrenze oder Eligibility-Regel dürfen erst verändert werden,
wenn eine Zustandskopie-Ablation einen dieser Parameter als eigenständigen
Engpass ausweist. E1, No-Learning, Bestätigung, Retention, Umweltumkehr und
vollständige Persistenz bleiben Pflichtkontrollen.

