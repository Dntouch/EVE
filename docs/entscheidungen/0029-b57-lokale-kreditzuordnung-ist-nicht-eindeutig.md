# 0029 – B.5.7: Lokale Kreditzuordnung ist zeitlich korrekt, aber kausal nicht eindeutig

## Status

Angenommen.

## Entscheidung

B.5.7 wird ohne neue Produktions-Lernregel zum Merge empfohlen. Die neue
Plastizitätsbeobachtung bleibt rein diagnostisch; S4 bleibt standardmäßig aus.
K13 und K14 werden nicht freigegeben.

## Begründung

Im Referenzraster ist bei jedem lokalen Modulationskontakt genau ein
Eligibility-Eintrag aktiv. Weder erfahrungsübergreifende Einträge noch
tatsächliche Mehrfachabschwächungen wurden beobachtet. Dennoch waren 1.888 von
9.444 bestätigten negativen sensorischen Änderungen für die aktuell bewertete
Handlung nicht kausal notwendig. Fast alle negativen Änderungen folgten einer
fehlenden Motorantwort. Damit liegt das Restproblem in der groben lokalen
Kandidatenzuordnung, nicht in doppelter Anwendung derselben Eligibility.

## Folgen

- Kein automatischer Schutz erfolgreicher Synapsen.
- Kein Eligibility-Rollback und keine Deduplicierungsregel.
- B.5.8 darf gezielt die kausale Eindeutigkeit negativer Kandidaten erforschen.
- Zeitraster und externe Erfahrungslabels bleiben reine Versuchsvariablen.
