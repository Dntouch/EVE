# Entscheidung 0026 – vorhandene Recovery ist nützlich, aber nicht ausreichend

## Status

Bestätigt am 19. November 2026.

## Entscheidung

Die vorhandenen unabhängigen Recoverymodi bleiben experimentell und werden
nicht als allgemeiner Produktionsstandard freigegeben. Eine neue Funktion
K12 und ihre Abschaltkontrolle K13 werden in B.5.4 nicht implementiert.

B.5.5 darf gezielt untersuchen, wie lokale, unterschwellige Membranzustände
ereignisgesteuert und ressourcenschonend zeitlich begrenzt werden können. Ein
globaler Tick, ein globaler Reset, die Kenntnis richtiger Handlungen oder die
Wiederherstellung früher erfolgreicher Zustände bleiben ausgeschlossen.

## Begründung

R1 und R2 sind bei aktivem Refraktärzustand technisch wirksam, persistent und
deterministisch. R2-50 erhöht in unabhängiger Bestätigung die gemeinsame
Abrufbarkeit gegenüber R1 um 18,12 Prozentpunkte und reduziert Verluste. Auch
Umweltumkehr und geschützte Exploration bleiben möglich.

Keine Variante erreicht jedoch die vorgegebene Robustheit von 48/64 Seeds je
Stromfamilie und Zuordnung. Zudem plant die bestehende Implementierung nur
nach einem Feuer ein Recoveryereignis. Nicht feuernde Subthreshold-Potenziale,
die spätere Entscheidungen beeinflussen, bleiben grundsätzlich unerreicht.

## Folgen

- Die historische R0-Referenz bleibt unverändert.
- Die technische Resetbeschreibung wird auf `to_zero` korrigiert.
- R1/R2 dürfen weiter als klar gekennzeichnete Forschungsvarianten dienen.
- B.5.5 untersucht den nachgewiesenen Reichweitenmangel, nicht eine
  nachträgliche Optimierung der B.5.4-Trefferquote.
- Jede neue Variante benötigt weiterhin eine Abschaltkontrolle und die
  vollständigen Robustheits-, Integrations-, Lern-, Persistenz- und
  Ressourcennachweise.
