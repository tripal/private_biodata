[![Build Status](https://travis-ci.org/UofS-Pulse-Binfo/private_biodata.svg?branch=master)](https://travis-ci.org/UofS-Pulse-Binfo/private_biodata)
![Tripal Dependency](https://img.shields.io/badge/tripal-%3E=3.0-brightgreen)
![Version](https://img.shields.io/badge/version-DEVELOPMENT-yellow)

# Private Tripal Content

Do you need a way to keep some data private while other data remains public? Tripal already provides the ability to make some Tripal content types private while others are public (e.g. all genetic maps are private but genes are public) but what if you only want some genetic maps public? This module was made to help in that exact case!

**Make individual pages public while others of the same Tripal Content Type remain private!**

This module provides an additional permission, `[Content type]: View Public Content` for each Tripal Content Type and a TripalField to indicate which exact pages should be public. Specifically, if you have checked "Release Publically" on a given Tripal Content Page edit form, all users with the `[Content type]: View Public Content` permission will be able to see it and if not, only users with the pre-existing `[Content type]: View Content` will be able to see it. I suggest setting `[Content type]: View Public Content` for anonymous users and `[Content type]: View Content` for authenticated users or, even better, a custom role.

**This module is still under active development**
