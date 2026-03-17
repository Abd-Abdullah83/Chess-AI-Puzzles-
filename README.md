<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>Chess Master — README</title>
<link href="https://fonts.googleapis.com/css2?family=Cinzel:wght@400;600;900&family=Crimson+Pro:ital,wght@0,300;0,400;0,600;1,300;1,400&family=JetBrains+Mono:wght@400;500&display=swap" rel="stylesheet">
<style>
  :root {
    --gold: #c9a84c;
    --gold-light: #e8c87a;
    --gold-dim: #7a5e20;
    --ivory: #f0ead6;
    --ivory-dim: #b8ad8e;
    --dark: #0a0804;
    --dark-2: #110e08;
    --dark-3: #1a1510;
    --dark-4: #241f16;
    --dark-5: #2e271b;
    --green: #2d5a27;
    --green-light: #4a9940;
    --red: #8b1a1a;
    --blue: #1a3a6b;
    --border: rgba(201,168,76,0.2);
    --border-bright: rgba(201,168,76,0.5);
  }

  * { box-sizing: border-box; margin: 0; padding: 0; }

  body {
    background: var(--dark);
    color: var(--ivory);
    font-family: 'Crimson Pro', Georgia, serif;
    font-size: 17px;
    line-height: 1.7;
    overflow-x: hidden;
  }

  /* Chess board background pattern */
  body::before {
    content: '';
    position: fixed;
    inset: 0;
    background-image:
      repeating-conic-gradient(rgba(201,168,76,0.03) 0% 25%, transparent 0% 50%);
    background-size: 60px 60px;
    pointer-events: none;
    z-index: 0;
  }

  .container {
    max-width: 900px;
    margin: 0 auto;
    padding: 0 2rem 4rem;
    position: relative;
    z-index: 1;
  }

  /* ── HERO ── */
  .hero {
    text-align: center;
    padding: 5rem 0 3rem;
    position: relative;
  }

  .hero-piece {
    font-size: 72px;
    display: block;
    margin-bottom: 1rem;
    animation: floatPiece 4s ease-in-out infinite;
    filter: drop-shadow(0 0 30px rgba(201,168,76,0.4));
  }

  @keyframes floatPiece {
    0%, 100% { transform: translateY(0); }
    50% { transform: translateY(-12px); }
  }

  h1 {
    font-family: 'Cinzel', serif;
    font-size: clamp(2.8rem, 6vw, 4.5rem);
    font-weight: 900;
    letter-spacing: 0.12em;
    text-transform: uppercase;
    color: var(--gold);
    line-height: 1;
    margin-bottom: 0.5rem;
    text-shadow: 0 0 60px rgba(201,168,76,0.3);
  }

  .hero-sub {
    font-family: 'Cinzel', serif;
    font-size: 0.85rem;
    letter-spacing: 0.35em;
    color: var(--ivory-dim);
    text-transform: uppercase;
    margin-bottom: 2rem;
  }

  .hero-desc {
    font-size: 1.05rem;
    color: var(--ivory-dim);
    font-style: italic;
    max-width: 580px;
    margin: 0 auto 2.5rem;
    font-weight: 300;
  }

  /* Badges */
  .badges {
    display: flex;
    flex-wrap: wrap;
    justify-content: center;
    gap: 8px;
    margin-bottom: 1rem;
  }

  .badge {
    font-family: 'JetBrains Mono', monospace;
    font-size: 10px;
    padding: 4px 10px;
    border-radius: 3px;
    font-weight: 500;
    letter-spacing: 0.05em;
    border: 1px solid;
  }

  .badge.cpp    { background: rgba(45,90,39,0.3); border-color: #4a9940; color: #7ee887; }
  .badge.sfml   { background: rgba(26,58,107,0.3); border-color: #3a6abf; color: #79b0ff; }
  .badge.ai     { background: rgba(139,26,26,0.3); border-color: #bf3a3a; color: #ff9090; }
  .badge.puzzle { background: rgba(122,94,32,0.3); border-color: var(--gold-dim); color: var(--gold-light); }
  .badge.audio  { background: rgba(50,30,70,0.3); border-color: #7a4abf; color: #bf9aff; }

  /* Gold divider */
  .divider {
    display: flex;
    align-items: center;
    gap: 1rem;
    margin: 2.5rem 0;
  }

  .divider::before, .divider::after {
    content: '';
    flex: 1;
    height: 1px;
    background: linear-gradient(90deg, transparent, var(--gold-dim), transparent);
  }

  .divider-icon {
    color: var(--gold);
    font-size: 1.1rem;
  }

  /* Section headings */
  h2 {
    font-family: 'Cinzel', serif;
    font-size: 1.1rem;
    font-weight: 600;
    letter-spacing: 0.2em;
    text-transform: uppercase;
    color: var(--gold);
    margin-bottom: 1.2rem;
    padding-bottom: 0.5rem;
    border-bottom: 1px solid var(--border);
  }

  h3 {
    font-family: 'Cinzel', serif;
    font-size: 0.85rem;
    font-weight: 600;
    letter-spacing: 0.15em;
    text-transform: uppercase;
    color: var(--ivory-dim);
    margin: 1.5rem 0 0.8rem;
  }

  p { margin-bottom: 1rem; color: var(--ivory); }

  /* ── MODE CARDS ── */
  .mode-grid {
    display: grid;
    grid-template-columns: 1fr 1fr;
    gap: 1px;
    background: var(--border);
    border: 1px solid var(--border);
    border-radius: 6px;
    overflow: hidden;
    margin: 1.2rem 0;
  }

  .mode-card {
    background: var(--dark-3);
    padding: 1.2rem 1.4rem;
    transition: background 0.2s;
  }

  .mode-card:hover { background: var(--dark-4); }

  .mode-card .icon {
    font-size: 1.4rem;
    margin-bottom: 0.4rem;
    display: block;
  }

  .mode-card .title {
    font-family: 'Cinzel', serif;
    font-size: 0.75rem;
    letter-spacing: 0.12em;
    text-transform: uppercase;
    font-weight: 600;
    margin-bottom: 0.35rem;
  }

  .mode-card.hvh .title  { color: #7ee887; }
  .mode-card.hvai .title { color: #79b0ff; }
  .mode-card.hvab .title { color: #ff9090; }
  .mode-card.pzl .title  { color: #bf9aff; }

  .mode-card .desc {
    font-size: 0.88rem;
    color: var(--ivory-dim);
    line-height: 1.5;
  }

  /* ── DIFFICULTY BARS ── */
  .diff-row {
    display: grid;
    grid-template-columns: 1fr 1fr 1fr;
    gap: 10px;
    margin: 1.2rem 0;
  }

  .diff-card {
    border: 1px solid;
    border-radius: 5px;
    padding: 1rem;
    text-align: center;
    position: relative;
    overflow: hidden;
  }

  .diff-card::before {
    content: '';
    position: absolute;
    bottom: 0; left: 0; right: 0;
    height: 3px;
  }

  .diff-card.easy   { background: rgba(45,90,39,0.15); border-color: #2d5a27; }
  .diff-card.easy::before { background: #4a9940; }
  .diff-card.medium { background: rgba(122,94,32,0.15); border-color: var(--gold-dim); }
  .diff-card.medium::before { background: var(--gold); }
  .diff-card.hard   { background: rgba(139,26,26,0.15); border-color: #8b1a1a; }
  .diff-card.hard::before { background: #e05050; }

  .diff-card .diff-label {
    font-family: 'Cinzel', serif;
    font-size: 0.75rem;
    letter-spacing: 0.2em;
    text-transform: uppercase;
    font-weight: 600;
    margin-bottom: 0.3rem;
  }

  .diff-card.easy .diff-label   { color: #7ee887; }
  .diff-card.medium .diff-label { color: var(--gold-light); }
  .diff-card.hard .diff-label   { color: #ff9090; }

  .diff-card .diff-desc {
    font-size: 0.82rem;
    color: var(--ivory-dim);
    font-style: italic;
  }

  /* ── TABLES ── */
  table {
    width: 100%;
    border-collapse: collapse;
    font-size: 0.88rem;
    margin: 1rem 0;
    border: 1px solid var(--border);
    border-radius: 5px;
    overflow: hidden;
  }

  th {
    background: var(--dark-4);
    color: var(--gold);
    font-family: 'Cinzel', serif;
    font-size: 0.72rem;
    letter-spacing: 0.15em;
    text-transform: uppercase;
    padding: 0.7rem 1rem;
    text-align: left;
    border-bottom: 1px solid var(--border-bright);
    font-weight: 600;
  }

  td {
    padding: 0.65rem 1rem;
    border-bottom: 1px solid var(--border);
    color: var(--ivory);
    vertical-align: top;
  }

  tr:last-child td { border-bottom: none; }
  tr:hover td { background: rgba(201,168,76,0.04); }

  /* ── CODE ── */
  code {
    font-family: 'JetBrains Mono', monospace;
    font-size: 0.78rem;
    background: rgba(201,168,76,0.08);
    color: var(--gold-light);
    padding: 2px 6px;
    border-radius: 3px;
    border: 1px solid rgba(201,168,76,0.15);
  }

  .code-block {
    background: var(--dark-2);
    border: 1px solid var(--border);
    border-radius: 6px;
    overflow: hidden;
    margin: 1rem 0;
  }

  .code-header {
    background: var(--dark-4);
    border-bottom: 1px solid var(--border);
    padding: 0.5rem 1rem;
    font-family: 'JetBrains Mono', monospace;
    font-size: 0.72rem;
    color: var(--ivory-dim);
    display: flex;
    align-items: center;
    gap: 8px;
  }

  .code-dot { width: 10px; height: 10px; border-radius: 50%; }
  .code-dot.r { background: #ff5f57; }
  .code-dot.y { background: #febc2e; }
  .code-dot.g { background: #28c840; }

  .code-body {
    padding: 1rem 1.2rem;
    overflow-x: auto;
  }

  .code-body pre {
    font-family: 'JetBrains Mono', monospace;
    font-size: 0.8rem;
    line-height: 1.7;
    color: #c0b090;
    white-space: pre;
    margin: 0;
  }

  .kw  { color: #ff7b72; }
  .ty  { color: #79c0ff; }
  .str { color: #a5d6ff; }
  .cm  { color: #666050; font-style: italic; }
  .fn  { color: #d2a8ff; }
  .num { color: var(--gold-light); }

  /* ── PUZZLE TIER CARDS ── */
  .puzzle-grid {
    display: grid;
    grid-template-columns: 1fr 1fr;
    gap: 10px;
    margin: 1rem 0;
  }

  .pt {
    background: var(--dark-3);
    border: 1px solid var(--border);
    border-radius: 5px;
    padding: 1rem 1.2rem;
    border-left: 3px solid;
  }

  .pt.easy   { border-left-color: #4a9940; }
  .pt.inter  { border-left-color: #3a6abf; }
  .pt.hard   { border-left-color: #bf6a20; }
  .pt.ultra  { border-left-color: #bf3a3a; }

  .pt-name {
    font-family: 'Cinzel', serif;
    font-size: 0.78rem;
    letter-spacing: 0.12em;
    text-transform: uppercase;
    font-weight: 600;
    margin-bottom: 0.3rem;
  }

  .pt.easy .pt-name  { color: #7ee887; }
  .pt.inter .pt-name { color: #79b0ff; }
  .pt.hard .pt-name  { color: var(--gold-light); }
  .pt.ultra .pt-name { color: #ff9090; }

  .pt-desc { font-size: 0.85rem; color: var(--ivory-dim); font-style: italic; }

  /* ── VIDEO LINKS ── */
  .video-grid {
    display: grid;
    grid-template-columns: 1fr 1fr;
    gap: 10px;
    margin: 1rem 0;
  }

  .video-card {
    background: var(--dark-3);
    border: 1px solid var(--border);
    border-radius: 6px;
    padding: 1rem 1.2rem;
    text-decoration: none;
    display: block;
    transition: border-color 0.2s, background 0.2s;
    position: relative;
    overflow: hidden;
  }

  .video-card::before {
    content: '▶';
    position: absolute;
    right: 1rem;
    top: 50%;
    transform: translateY(-50%);
    color: var(--gold-dim);
    font-size: 0.9rem;
    transition: color 0.2s;
  }

  .video-card:hover { border-color: var(--gold); background: var(--dark-4); }
  .video-card:hover::before { color: var(--gold); }

  .video-mode {
    font-family: 'Cinzel', serif;
    font-size: 0.72rem;
    letter-spacing: 0.15em;
    text-transform: uppercase;
    color: var(--gold);
    margin-bottom: 0.2rem;
  }

  .video-file {
    font-family: 'JetBrains Mono', monospace;
    font-size: 0.75rem;
    color: var(--ivory-dim);
  }

  /* ── TREE ── */
  .tree {
    background: var(--dark-2);
    border: 1px solid var(--border);
    border-radius: 6px;
    padding: 1.2rem 1.5rem;
    font-family: 'JetBrains Mono', monospace;
    font-size: 0.8rem;
    line-height: 2;
    margin: 1rem 0;
  }

  .tree .dir  { color: #79b0ff; }
  .tree .cm2  { color: #555040; font-style: italic; }
  .tree .cpp  { color: var(--gold-light); }
  .tree .mp4  { color: #bf9aff; }
  .tree .misc { color: var(--ivory-dim); }

  /* ── CALLOUTS ── */
  .callout {
    border-left: 3px solid;
    border-radius: 0 5px 5px 0;
    padding: 0.8rem 1.2rem;
    margin: 1rem 0;
    font-size: 0.9rem;
  }

  .callout.note { border-color: #3a6abf; background: rgba(58,106,191,0.08); }
  .callout.warn { border-color: #bf8a20; background: rgba(191,138,32,0.08); }

  .callout-title {
    font-family: 'Cinzel', serif;
    font-size: 0.72rem;
    letter-spacing: 0.15em;
    text-transform: uppercase;
    font-weight: 600;
    margin-bottom: 0.3rem;
  }

  .callout.note .callout-title { color: #79b0ff; }
  .callout.warn .callout-title { color: var(--gold-light); }
  .callout p { margin: 0; color: var(--ivory-dim); font-size: 0.88rem; }

  /* ── AUDIO TAGS ── */
  .audio-row {
    display: flex;
    flex-wrap: wrap;
    gap: 7px;
    margin: 0.8rem 0;
  }

  .atag {
    font-family: 'JetBrains Mono', monospace;
    font-size: 0.75rem;
    padding: 3px 10px;
    border-radius: 3px;
    background: rgba(50,30,70,0.4);
    border: 1px solid rgba(122,74,191,0.35);
    color: #bf9aff;
  }

  /* ── FOOTER ── */
  .footer {
    text-align: center;
    padding: 3rem 0 1rem;
    color: var(--ivory-dim);
    font-size: 0.85rem;
    font-style: italic;
  }

  .footer .gold { color: var(--gold); }

  /* ── ANIMATIONS ── */
  @keyframes fadeUp {
    from { opacity: 0; transform: translateY(20px); }
    to   { opacity: 1; transform: translateY(0); }
  }

  .hero        { animation: fadeUp 0.7s ease both; }
  .hero-piece  { animation: fadeUp 0.5s ease both, floatPiece 4s ease-in-out 0.5s infinite; }
  section      { animation: fadeUp 0.6s ease both; }

  section:nth-child(2)  { animation-delay: 0.05s; }
  section:nth-child(3)  { animation-delay: 0.10s; }
  section:nth-child(4)  { animation-delay: 0.15s; }
  section:nth-child(5)  { animation-delay: 0.20s; }
  section:nth-child(6)  { animation-delay: 0.25s; }
  section:nth-child(7)  { animation-delay: 0.30s; }
  section:nth-child(8)  { animation-delay: 0.35s; }
  section:nth-child(9)  { animation-delay: 0.40s; }
  section:nth-child(10) { animation-delay: 0.45s; }

  @media (max-width: 600px) {
    .mode-grid, .diff-row, .puzzle-grid, .video-grid { grid-template-columns: 1fr; }
    h1 { font-size: 2.2rem; }
  }
</style>
</head>
<body>
<div class="container">

  <!-- HERO -->
  <div class="hero">
    <span class="hero-piece">♟</span>
    <h1>Chess Master</h1>
    <p class="hero-sub">C++ · SFML · Minimax AI · Puzzle System</p>
    <p class="hero-desc">A full-featured graphical chess game with Human vs Human, Human vs AI across three difficulty levels, and a rated puzzle training mode with four tiers.</p>
    <div class="badges">
      <span class="badge cpp">C++17</span>
      <span class="badge sfml">SFML 2.x</span>
      <span class="badge ai">Minimax + Alpha-Beta</span>
      <span class="badge puzzle">Puzzle Rating System</span>
      <span class="badge audio">SFML Audio</span>
    </div>
  </div>

  <!-- DEMO VIDEOS -->
  <section>
    <div class="divider"><span class="divider-icon">◈</span></div>
    <h2>Demo Videos</h2>
    <div class="video-grid">
      <a class="video-card" href="ChessV%20(1).mp4">
        <div class="video-mode">Full Gameplay</div>
        <div class="video-file">ChessV (1).mp4</div>
      </a>
      <a class="video-card" href="Human%20VS%20Ai%20.mp4">
        <div class="video-mode">Human vs AI</div>
        <div class="video-file">Human VS Ai .mp4</div>
      </a>
      <a class="video-card" href="HumanvsHuman.mp4">
        <div class="video-mode">Human vs Human</div>
        <div class="video-file">HumanvsHuman.mp4</div>
      </a>
      <a class="video-card" href="ChessPuzzle.mp4">
        <div class="video-mode">Puzzle Mode</div>
        <div class="video-file">ChessPuzzle.mp4</div>
      </a>
    </div>
  </section>

  <!-- GAME MODES -->
  <section>
    <div class="divider"><span class="divider-icon">◈</span></div>
    <h2>Game Modes</h2>
    <div class="mode-grid">
      <div class="mode-card hvh">
        <span class="icon">♙</span>
        <div class="title">Human vs Human</div>
        <div class="desc">Local 2-player on the same machine. Full chess rules enforced — castling, en passant, promotion.</div>
      </div>
      <div class="mode-card hvai">
        <span class="icon">♖</span>
        <div class="title">Human vs AI (White)</div>
        <div class="desc">Play as Black against the engine. Choose Easy, Medium, or Hard difficulty before the game.</div>
      </div>
      <div class="mode-card hvab">
        <span class="icon">♜</span>
        <div class="title">Human vs AI (Black)</div>
        <div class="desc">Play as White against the engine. The AI opens with calculated moves at higher difficulties.</div>
      </div>
      <div class="mode-card pzl">
        <span class="icon">♛</span>
        <div class="title">Puzzle Mode</div>
        <div class="desc">Rated puzzle training across 4 tiers. Earn rating, track streaks, and use hints.</div>
      </div>
    </div>
  </section>

  <!-- AI ENGINE -->
  <section>
    <div class="divider"><span class="divider-icon">◈</span></div>
    <h2>AI Engine</h2>
    <p>The AI uses <strong>Minimax with Alpha-Beta pruning</strong> and a <strong>quiescence search</strong> extension to prevent horizon-effect blunders on captures. A 1.8-second hard time limit per move keeps gameplay smooth at all depths.</p>
    <div class="diff-row">
      <div class="diff-card easy">
        <div class="diff-label">Easy</div>
        <div class="diff-desc">Shallow depth — makes occasional tactical mistakes</div>
      </div>
      <div class="diff-card medium">
        <div class="diff-label">Medium</div>
        <div class="diff-desc">Balanced — solid tactics, some strategic awareness</div>
      </div>
      <div class="diff-card hard">
        <div class="diff-label">Hard</div>
        <div class="diff-desc">Deep search + quiescence — aggressive and accurate</div>
      </div>
    </div>
  </section>

  <!-- CHESS RULES -->
  <section>
    <div class="divider"><span class="divider-icon">◈</span></div>
    <h2>Chess Rules Implemented</h2>
    <table>
      <tr><th>Rule</th><th>Implementation Detail</th></tr>
      <tr><td>Castling</td><td>Kingside &amp; queenside for both colours — rook and king move flags tracked individually</td></tr>
      <tr><td>En passant</td><td>Full double-pawn push detection; target square stored and cleared each turn</td></tr>
      <tr><td>Pawn promotion</td><td>Interactive promotion menu auto-appears when a pawn reaches the 8th rank</td></tr>
      <tr><td>Check detection</td><td>Every move validated to ensure the moving side's king is not left in check</td></tr>
      <tr><td>Checkmate &amp; stalemate</td><td>Detected and announced at end of each turn with distinct sound effects</td></tr>
      <tr><td>Undo / Redo</td><td>Full <code>GameState</code> stack — unlimited undo and redo during a game</td></tr>
      <tr><td>Captured pieces</td><td>White and black captured pieces tracked separately throughout the game</td></tr>
    </table>
  </section>

  <!-- PUZZLE SYSTEM -->
  <section>
    <div class="divider"><span class="divider-icon">◈</span></div>
    <h2>Puzzle System</h2>
    <p>A complete standalone engine (<code>ChessPuzzleSystem</code>) with FEN-based positions, move validation in SAN notation, Elo-style rating, streaks, and hint support.</p>
    <div class="puzzle-grid">
      <div class="pt easy">
        <div class="pt-name">Easy</div>
        <div class="pt-desc">Beginner tactics — one-move mates and simple forks</div>
      </div>
      <div class="pt inter">
        <div class="pt-name">Intermediate</div>
        <div class="pt-desc">Two-move combinations, pins, and skewers</div>
      </div>
      <div class="pt hard">
        <div class="pt-name">Hard</div>
        <div class="pt-desc">Multi-move sequences and piece sacrifices</div>
      </div>
      <div class="pt ultra">
        <div class="pt-name">Ultra Hard</div>
        <div class="pt-desc">Deep calculation, endgame studies, complex tactics</div>
      </div>
    </div>

    <h3>Key Methods</h3>
    <div class="code-block">
      <div class="code-header">
        <div class="code-dot r"></div><div class="code-dot y"></div><div class="code-dot g"></div>
        <span>ChessPuzzleSystem.h</span>
      </div>
      <div class="code-body"><pre><span class="ty">void</span>          <span class="fn">initializePuzzles</span>();
<span class="ty">Puzzle</span>        <span class="fn">getNextPuzzle</span>(<span class="ty">PuzzleDifficulty</span> difficulty);
<span class="ty">void</span>          <span class="fn">startPuzzle</span>(<span class="kw">const</span> <span class="ty">Puzzle</span>&amp; puzzle);
<span class="ty">PuzzleResult</span>  <span class="fn">checkMove</span>(<span class="kw">const</span> <span class="ty">string</span>&amp; move);
<span class="ty">string</span>        <span class="fn">getHint</span>();
<span class="ty">PuzzleResult</span>  <span class="fn">completePuzzle</span>(<span class="ty">bool</span> solved);</pre></div>
    </div>

    <h3>Result Tracking</h3>
    <table>
      <tr><th>Field</th><th>Description</th></tr>
      <tr><td><code>timeTaken</code></td><td>Seconds from puzzle start to final move</td></tr>
      <tr><td><code>attempts</code></td><td>Up to 3 attempts per puzzle</td></tr>
      <tr><td><code>hintsUsed</code></td><td>Reduces rating gain when non-zero</td></tr>
      <tr><td><code>ratingChange</code> / <code>bonusPoints</code></td><td>Elo-style delta + streak bonus points</td></tr>
      <tr><td><code>streak</code></td><td>Consecutive-day solving streak</td></tr>
      <tr><td><code>oldRating</code> / <code>newRating</code></td><td>Rating before and after the puzzle</td></tr>
    </table>
  </section>

  <!-- SOUND -->
  <section>
    <div class="divider"><span class="divider-icon">◈</span></div>
    <h2>Sound Design</h2>
    <p>All effects load from <code>audio/</code>. Missing files fall back gracefully — the game always runs.</p>
    <div class="audio-row">
      <span class="atag">move</span>
      <span class="atag">capture</span>
      <span class="atag">castling</span>
      <span class="atag">checkmate</span>
      <span class="atag">stalemate</span>
      <span class="atag">game start</span>
      <span class="atag">puzzle complete</span>
      <span class="atag">background music (loop)</span>
    </div>
  </section>

  <!-- STRUCTURE -->
  <section>
    <div class="divider"><span class="divider-icon">◈</span></div>
    <h2>Project Structure</h2>
    <div class="tree">
<span class="dir">Chess-AI-Puzzles/</span>
├── <span class="cpp">Chess.cpp</span>               <span class="cm2">// Rendering, input, AI, game loop</span>
├── <span class="cpp">ChessPuzzleSystem.cpp</span>   <span class="cm2">// Puzzle engine implementation</span>
├── <span class="cpp">ChessPuzzleSystem.h</span>     <span class="cm2">// Structs, enums, class declaration</span>
├── <span class="mp4">ChessPuzzle.mp4</span>         <span class="cm2">// Puzzle mode demo</span>
├── <span class="mp4">ChessV (1).mp4</span>          <span class="cm2">// Full gameplay demo</span>
├── <span class="mp4">Human VS Ai .mp4</span>        <span class="cm2">// Human vs AI demo</span>
├── <span class="mp4">HumanvsHuman.mp4</span>        <span class="cm2">// Human vs Human demo</span>
├── <span class="dir">pieces/</span>                 <span class="cm2">// Piece sprite PNGs</span>
├── <span class="dir">audio/</span>                  <span class="cm2">// Sound effects + background music</span>
├── <span class="dir">Font/</span>
│   └── <span class="misc">arial.ttf</span>
├── <span class="misc">.gitignore</span>
└── <span class="misc">LICENSE</span>
    </div>
  </section>

  <!-- BUILD -->
  <section>
    <div class="divider"><span class="divider-icon">◈</span></div>
    <h2>Build &amp; Run</h2>

    <h3>Linux / macOS</h3>
    <div class="code-block">
      <div class="code-header">
        <div class="code-dot r"></div><div class="code-dot y"></div><div class="code-dot g"></div>
        <span>bash</span>
      </div>
      <div class="code-body"><pre>git clone https://github.com/Abd-Abdullah83/Chess-AI-Puzzles-.git
cd Chess-AI-Puzzles-

g++ -std=c++17 Chess.cpp ChessPuzzleSystem.cpp -o chess \
    -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

./chess</pre></div>
    </div>

    <h3>Windows (MinGW)</h3>
    <div class="code-block">
      <div class="code-header">
        <div class="code-dot r"></div><div class="code-dot y"></div><div class="code-dot g"></div>
        <span>cmd</span>
      </div>
      <div class="code-body"><pre>g++ -std=c++17 Chess.cpp ChessPuzzleSystem.cpp -o chess.exe ^
    -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

chess.exe</pre></div>
    </div>

    <div class="callout note">
      <div class="callout-title">Prerequisite</div>
      <p>SFML 2.x must be installed. Ubuntu: <code>sudo apt install libsfml-dev</code> · macOS: <code>brew install sfml</code> · Windows: download from sfml-dev.org and add to your MinGW library path.</p>
    </div>
    <div class="callout warn">
      <div class="callout-title">Asset Paths</div>
      <p>Run the executable from the <strong>project root</strong>. The game resolves <code>pieces/</code>, <code>audio/</code>, and <code>Font/arial.ttf</code> relative to the working directory.</p>
    </div>
  </section>

  <!-- CONTROLS -->
  <section>
    <div class="divider"><span class="divider-icon">◈</span></div>
    <h2>Controls</h2>
    <table>
      <tr><th>Action</th><th>Input</th></tr>
      <tr><td>Select &amp; move a piece</td><td>Left click — source square, then destination</td></tr>
      <tr><td>Undo move</td><td><strong>UNDO</strong> button — top-left of window</td></tr>
      <tr><td>Redo move</td><td><strong>REDO</strong> button — top-left of window</td></tr>
      <tr><td>Switch board theme</td><td>Slider — top-right corner</td></tr>
      <tr><td>Pawn promotion</td><td>Interactive menu on reaching the 8th rank</td></tr>
      <tr><td>Navigate menus</td><td>Left click on mode / difficulty buttons</td></tr>
    </table>
  </section>

  <!-- FOOTER -->
  <div class="divider"><span class="divider-icon">◈</span></div>
  <div class="footer">
    <p>Built by <span class="gold">Abd-Abdullah83</span> · C++ · SFML · FAST NUCES</p>
    <p style="margin-top:0.3rem; font-size:0.78rem; opacity:0.5;">Every position has a best move.</p>
  </div>

</div>
</body>
</html>
