# DTU Evaluation Code

This is a fixed and improved version of the original Matlab DTU evaluation code. Some of the changes:

- fix computed evaluation score to consider only points validated by the scene mask
- fix `MaxDistCP` to compute the true score, regardless of the set `MaxDist` value
- evaluation code now supports as input both mesh and points-cloud; `eval_ply` automatically detects the right mode
- re-implement some core components in C++ in order to speed-up evaluation (20x on average)

See an usage example in `runTest.bat`. Tested with Matlab R2019b trial version.
