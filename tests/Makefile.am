## -*- Makefile.am -*-
## Process this file with automake to produce Makefile.in
##
# Makefile for the tests in the Ratfor Testsuite.

## Some *.am files include later might require these special variables
## to be initialized, even if only with an empty value.
MOSTLYCLEANFILES =
EXTRA_DIST =

##
## List of tests.
##

EXTRA_DIST += $(TESTS)

# Defined at configure time.
XFAIL_TESTS = $(RAT4_TESTSUITE_XFAIL_TESTS)

TESTS = \
  balanced-parens.test \
  break-fail.test \
  break_f77.test \
  case-sensibility-basic.test \
  comments-discard-after-numerical-label.test \
  comments-discard-at-stmt-begin.test \
  comments-discard-in-stmt-middle.test \
  comments-keep-at-stmt-begin.test \
  comments-keep_f77.test \
  comments-no-extra-white-spaces.test \
  comments-no-hang.test \
  comments-not-in-strings.test \
  comments-unobtrusiveness.test \
  common-block.test \
  common-block_f77.test \
  data-statement.test \
  data-statement_f77.test \
  define-balanced-paren.test \
  define-balanced-paren_f77.test \
  define-basic.test \
  define-basic_f77.test \
  define-case-sensibility.test \
  define-comments-interaction-1.test \
  define-comments-interaction-2.test \
  define-comments-interaction-3.test \
  define-dollar-in-symbolic-name.test \
  define-fail.test \
  define-long-definition.test \
  define-long-name.test \
  define-multiline-with-comment.test \
  define-multiline.test \
  define-multiline_f77.test \
  define-nested.test \
  define-nested_f77.test \
  define-no-dot-in-symbolic-name.test \
  define-no-uppercase-alias.test \
  define-redefine.test \
  define-semicolon.test \
  define-semicolon_f77.test \
  define-stress_f77.test \
  define-underscore-in-symbolic-name.test \
  define-whitespaces.test \
  devnull-input.test \
  empty-input.test \
  exponentiation-operator_f77.test \
  for-with-empty-clauses_f77.test \
  for_f77.test \
  forever1_f77.test \
  forever2_f77.test \
  function-name-invalid-1.test \
  function-name-invalid-2.test \
  function-name-invalid-3.test \
  function-name-missing-1.test \
  function-name-missing-2.test \
  function-return.test \
  function-return_f77.test \
  goto_f77.test \
  if_f77.test \
  ifelse-ambiguity_f77.test \
  illegal-case-default.test \
  include-abspath.test \
  include-basic.test \
  include-case-sensible.test \
  include-cornercase.test \
  include-defined.test \
  include-fail-directory.test \
  include-fail-nonexistent.test \
  include-fail-unreadable.test \
  include-missing-filename.test \
  include-nested-deep.test \
  include-nested.test \
  include-no-uppercase-alias.test \
  include-nonalpha.test \
  include-recursive.test \
  include-relpath.test \
  include-stress.test \
  infile-directory-fail.test \
  infile-nonexistent-fail.test \
  infile-unreadable-fail.test \
  integers-many-bases.test \
  integers-many-bases_f77.test \
  label-conflict.test \
  label-fail-empty-statement.test \
  label-fail-misplaced-paren.test \
  label-fail-unexpected-eof.test \
  label-line-continuation.test \
  label-line-continuation_f77.test \
  line-continuation-automatic-operators.test \
  line-continuation-automatic-others.test \
  line-continuation-automatic-relationals-raw.test \
  line-continuation-automatic-relationals.test \
  line-continuation-basic.test \
  line-continuation-in-string.test \
  line-continuation-multiple.test \
  line-continuation-stress1.test \
  line-continuation-stress2.test \
  line-continuation_f77.test \
  long-infile.test \
  long-infile_f77.test \
  nested-loops_f77.test \
  nested-subprogram-fail.test \
  next-fail.test \
  next_f77.test \
  no-curly-brackets.test \
  no-double-quotes.test \
  no-fortran66-if.test \
  no-fortran66-while.test \
  no-semicolons.test \
  no-string-directive.test \
  no-tabs.test \
  non-alpha-tokens-1.test \
  non-alpha-tokens-2.test \
  outfile-cantcreate-fail.test \
  outfile-dash-is-stdout.test \
  outfile-devnull.test \
  outfile-directory-fail.test \
  outfile-no-multi-clobber.test \
  outfile-no-useless-overwrite.test \
  outfile-overwrite.test \
  outfile-unwritable-fail.test \
  outfile.test \
  repeat-until_f77.test \
  repeat-without-until.test \
  repeat-without-until_f77.test \
  return-a-value-from-subroutine-fail.test \
  return-keyword-function-1.test \
  return-keyword-function-2.test \
  return-keyword-function-and-subroutine.test \
  return-keyword-function_f77.test \
  return-keyword-outside-subprogram-fail.test \
  return-keyword-stress_f77.test \
  return-keyword-subroutine.test \
  return-keyword-subroutine_f77.test \
  simple-assignement.test \
  start-a-statement-curly-bracket.test \
  start-a-statement-semicolon.test \
  startlabel-default.test \
  startlabel-user-choice.test \
  stmt-stack-stress_f77.test \
  stress1_f77.test \
  stress2_f77.test \
  stress3_f77.test \
  string-concatenation_f77.test \
  subprogram-name-invalid-not-repeated.test \
  subroutine-name-invalid-1.test \
  subroutine-name-invalid-2.test \
  subroutine-name-invalid-3.test \
  subroutine-name-missing-1.test \
  subroutine-name-missing-2.test \
  switch-fail-case-label-duplicated.test \
  switch-fail-case-label-invalid.test \
  switch-fail-no-braces.test \
  switch-non-positive-values_f77.test \
  switch_f77.test \
  synerr-basic.test \
  synerr-define-lineno.test \
  synerr-include-lineno.test \
  synerr-lineno.test \
  synerr-nonfatal.test \
  synerr-nonfatal_f77.test \
  synerr-stdin.test \
  translate-relationals.test \
  usage-option-missing-argument.test \
  usage-option-unknown.test \
  usage-too-many-args.test \
  verbatim-at-stmt-beginning-at-least.test \
  verbatim-at-stmt-beginning-only.test \
  verbatim-curly-brackets.test \
  verbatim-define-stress-interaction.test \
  verbatim-in-define.test \
  verbatim-include-interaction.test \
  verbatim-long-line.test \
  verbatim-no-extra-new-line.test \
  verbatim-stress_f77.test \
  verbatim.test \
  verbatim_f77.test \
  while_f77.test


## Requirements and dependencies of tests.

EXTRA_DIST += integers-many-bases.data
integers-many-bases.log: integers-many-bases.data
integers-many-bases_f77.log: integers-many-bases.data

EXTRA_DIST += break-next-fail.data
break-fail.test: break-next-fail.data
next-fail.test: break-next-fail.data

EXTRA_DIST += subprog-name-invalid.sh
subroutine-name-missing-1.log: subprog-name-invalid.sh
subroutine-name-missing-2.log: subprog-name-invalid.sh
subroutine-name-invalid-1.log: subprog-name-invalid.sh
subroutine-name-invalid-2.log: subprog-name-invalid.sh
subroutine-name-invalid-3.log: subprog-name-invalid.sh
function-name-missing-1.log: subprog-name-invalid.sh
function-name-missing-2.log: subprog-name-invalid.sh
function-name-invalid-1.log: subprog-name-invalid.sh
function-name-invalid-2.log: subprog-name-invalid.sh
function-name-invalid-3.log: subprog-name-invalid.sh

EXTRA_DIST += start-a-statement-sanitycheck.sh
start-a-statement-curly-bracket.log: start-a-statement-sanitycheck.sh
start-a-statement-semicolon.log: start-a-statement-sanitycheck.sh

EXTRA_DIST += comments-at-stmt-begin.data
comments-discard-at-stmt-begin.log: comments-at-stmt-begin.data
comments-keep-at-stmt-begin.log: comments-at-stmt-begin.data

EXTRA_DIST += define-comments-interaction.sh
define-comments-interaction-1.log: define-comments-interaction.sh
define-comments-interaction-2.log: define-comments-interaction.sh
define-comments-interaction-3.log: define-comments-interaction.sh

## Testsuite environment and setup.
include ../am/TestsEnvironment.am

## Testsuite cleanup.
include ../am/LocalCleanup.am

## Consistency checks on the list of tests.
include ../am/CheckListOfTests.am

## Work around a bug of FreeBSD make w.r.t. parallel tests.
include ../am/WorkaroundParallelTests.am

## vim: ft=automake noet sw=4 ts=4
