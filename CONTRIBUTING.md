# Beiträge zu EVE

Beiträge müssen die wissenschaftliche Nachvollziehbarkeit und die
Projektgrundsätze von EVE erhalten.

## Vor einer Änderung

1. Das Projektkonzept und die bestehenden Entscheidungen lesen.
2. Für eine Aufgabe, Forschungsfrage, Entscheidung oder ein Experiment ein
   passendes Issue anlegen.
3. Grundsatzfragen klären, bevor daraus Implementierungsarbeit entsteht.

## Anforderungen an Änderungen

- Motivation und erwartete Auswirkung sind beschrieben.
- Aussagen, Hypothesen und Ergebnisse werden klar voneinander getrennt.
- Wissenschaftliche Quellen werden mit dauerhafter Fundstelle angegeben.
- Neue Begriffe werden bei Bedarf im Glossar ergänzt.
- Geänderte Grundsätze erhalten ein Entscheidungsprotokoll.
- Experimente dokumentieren Ausgangszustand, Ereignisfolge, Zufallszustände,
  Messgrößen und Abbruchkriterien.
- Semantische Annahmen dürfen nicht unbemerkt in den neuronalen Kern gelangen.
- Ethische oder sicherheitsrelevante Auswirkungen werden ausdrücklich benannt.

## Git-Arbeitsweise

- Branches sind kurzlebig und auf ein Thema begrenzt.
- Empfohlene Namen: `docs/...`, `research/...`, `decision/...`, `experiment/...`.
- Commits enthalten eine in sich verständliche Änderung.
- Empfohlene Commit-Typen: `docs`, `research`, `decision`, `experiment`, `chore`.
- Pull Requests verweisen auf das zugehörige Issue und relevante Dokumente.
- Direkte Änderungen am Hauptbranch sollen vermieden werden.

Details stehen in [docs/ARBEITSWEISE.md](docs/ARBEITSWEISE.md).

## Sprache

Projektdokumentation wird zunächst auf Deutsch geführt. Technische Bezeichner
können englisch sein, müssen aber im Kontext eindeutig bleiben.

## Lizenz

Eine Projektlizenz ist noch nicht beschlossen. Beiträge dürfen deshalb erst
nach Klärung der Rechte und des beabsichtigten Veröffentlichungsmodells extern
übernommen werden.
