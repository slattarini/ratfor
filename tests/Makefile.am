## -*- Makefile.am -*-
## Process this file with automake to produce Makefile.in
##
# Makefile for the tests in the Ratfor Testsuite.

# Required to avoid make erronously seeing comments in shell code.
sharp = \#

## All updated later throughout the file.
TESTS =
check_SCRIPTS =
EXTRA_DIST =
MOSTLYCLEANFILES =
CLEANFILES =

# Defined at configure time.
XFAIL_TESTS = $(RAT4_TESTSUITE_XFAIL_TESTS)


##
## Hand-written tests.
##

EXTRA_DIST += $(handwritten_tests)
TESTS += $(handwritten_tests)
handwritten_tests = \
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
  comments-not-in-strings.test \
  comments-unobtrusiveness.test \
  common-block.test \
  common-block_f77.test \
  data-statement.test \
  data-statement_f77.test \
  define-at-stmt-beginning-at-least.test \
  define-at-stmt-beginning-only.test \
  define-balanced-paren-comments.test \
  define-balanced-paren-strings.test \
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
  define-semicolon-no-parens.test \
  define-semicolon-no-parens_f77.test \
  define-semicolon-parens.test \
  define-semicolon-parens_f77.test \
  define-stress_f77.test \
  define-underscore-in-symbolic-name.test \
  define-with-parens-preserve-whitespace.test \
  devnull-input.test \
  else-without-if-fail.test \
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
  include-at-stmt-beginning-at-least-dquote.test \
  include-at-stmt-beginning-at-least-plain.test \
  include-at-stmt-beginning-at-least-squote.test \
  include-at-stmt-beginning-only-dquote.test \
  include-at-stmt-beginning-only-plain.test \
  include-at-stmt-beginning-only-squote.test \
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
  labeled-format.test \
  labeled-format_f77.test \
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
  misc1.test \
  nested-loops_f77.test \
  nested-subprogram-fail.test \
  next-fail.test \
  next_f77.test \
  no-curly-brackets.test \
  no-double-quotes.test \
  no-extra-leading-white-spaces.test \
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
  repeat-nested.test \
  repeat-nested_f77.test \
  repeat-until-ambiguity_f77.test \
  repeat-until-run-body-at-least-once_f77.test \
  repeat-until_f77.test \
  repeat-without-until.test \
  repeat-without-until-2.test \
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
  stress4_f77.test \
  stress5_f77.test \
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
  until-after-empty-lines.test \
  until-with-extra-traling-tokens-fail.test \
  until-with-statement-terminator.test \
  until-with-trailing-blanks.test \
  until-without-repeat-fail.test \
  usage-option-missing-argument.test \
  usage-option-unknown.test \
  usage-too-many-args.test \
  verbatim-at-stmt-beginning-at-least.test \
  verbatim-at-stmt-beginning-only.test \
  verbatim-basic.test \
  verbatim-basic_f77.test \
  verbatim-curly-brackets.test \
  verbatim-define-interaction-basic-gentler.test \
  verbatim-define-interaction-basic.test \
  verbatim-define-interaction-stress-gentler.test \
  verbatim-define-interaction-stress.test \
  verbatim-include-interaction.test \
  verbatim-long-line.test \
  verbatim-no-extra-new-line.test \
  verbatim-stress_f77.test \
  while_f77.test


##
## Requirements and dependencies of tests.
##

EXTRA_DIST += integers-many-bases.data
integers-many-bases.log integers-many-bases_f77.log: \
  integers-many-bases.data

EXTRA_DIST += break-next-fail.data
break-fail.test next-fail.test: break-next-fail.data

EXTRA_DIST += subprog-name-invalid.sh
subroutine-name-missing-1.log \
subroutine-name-missing-2.log \
subroutine-name-invalid-1.log \
subroutine-name-invalid-2.log \
subroutine-name-invalid-3.log \
function-name-missing-1.log \
function-name-missing-2.log \
function-name-invalid-1.log \
function-name-invalid-2.log \
function-name-invalid-3.log \
  : subprog-name-invalid.sh

EXTRA_DIST += start-a-statement-sanitycheck.sh
start-a-statement-curly-bracket.log start-a-statement-semicolon.log: \
  start-a-statement-sanitycheck.sh

EXTRA_DIST += comments-at-stmt-begin.data
comments-discard-at-stmt-begin.log comments-keep-at-stmt-begin.log: \
  comments-at-stmt-begin.data

EXTRA_DIST += define-comments-interaction.sh
define-comments-interaction-1.log \
define-comments-interaction-2.log \
define-comments-interaction-3.log \
  : define-comments-interaction.sh

EXTRA_DIST += include-at-stmt-beginning-at-least.sh
include-at-stmt-beginning-at-least-dquote.log \
include-at-stmt-beginning-at-least-plain.log \
include-at-stmt-beginning-at-least-squote.log \
  : include-at-stmt-beginning-at-least.sh

EXTRA_DIST += include-at-stmt-beginning-only.sh
include-at-stmt-beginning-only-dquote.log \
include-at-stmt-beginning-only-plain.log \
include-at-stmt-beginning-only-squote.log \
  : include-at-stmt-beginning-only.sh


##
## Generated tests.
##

#
# General scaffolding.
#

generated_tests = ## updated later
check_SCRIPTS += $(generated_tests)
TESTS += $(generated_tests)
CLEANFILES += $(generated_tests)
MOSTLYCLEANFILES += *.test-t

## helper internal variables
autogen_sh__pre = $(AM_V_at)set -e;\
  rm -f $@ $@-t; \
  { \
    echo '$(sharp)! /bin/sh'; \
    echo '$(sharp) This file is part of the Ratfor Testsuite.'; \
    echo '$(sharp) This file has been automatically generated. ' \
         'DO NOT EDIT BY HAND!'; \
    echo; \
    echo '. $(top_builddir)/lib/rat4-testsuite-init.sh || exit 99'; \
    echo; \
  } >$@-t

autogen_sh__post = $(AM_V_at)set -e;\
  echo >>$@-t; \
  echo '$(sharp) vim: ft=sh ts=4 sw=4 et' >>$@-t; \
  chmod a-w,a+x $@-t; \
  mv -f $@-t $@

#
# Ratfor control structures after numeric labels.
#

tests_control_structure_after_numeric_label = \
  if-else-after-numeric-label.test \
  if-after-numeric-label.test \
  while-after-numeric-label.test \
  for-after-numeric-label.test \
  do-after-numeric-label.test \
  repeat-after-numeric-label.test \
  repeat-until-after-numeric-label.test \
  switch-after-numeric-label.test \
  break-after-numeric-label.test \
  next-after-numeric-label.test
tests_control_structure_after_numeric_label_f77 = \
  $(tests_control_structure_after_numeric_label:.test=_f77.test)
generated_tests += \
  $(tests_control_structure_after_numeric_label) \
  $(tests_control_structure_after_numeric_label_f77)

EXTRA_DIST += control-structure-after-numeric-label.sh
$(tests_control_structure_after_numeric_label:.test=.log): \
  control-structure-after-numeric-label.sh

EXTRA_DIST += control-structure-after-numeric-label_f77.sh
$(tests_control_structure_after_numeric_label_f77:.test=.log): \
  control-structure-after-numeric-label_f77.sh

$(tests_control_structure_after_numeric_label) \
$(tests_control_structure_after_numeric_label_f77): Makefile
	$(autogen_sh__pre)
	$(AM_V_GEN)case '$@' in \
	   *_f77.test) a='_f77';; \
	   *.test) a='';; \
	   *) echo "Invalid script \`$@' to be generated" >&2; exit 1;; \
	 esac \
	   && helper="control-structure-after-numeric-label$$a.sh" \
	   && stmt=`echo $@ | sed "s/-after-numeric-label$$a\.test$$//"` \
	   && echo "stmt='$$stmt'" >> $@-t \
	   && echo "rat4t_require_helper_script '$$helper'" >> $@-t
	$(autogen_sh__post)

#
# Ratfor keywords in the middle of statements.
#

tests_keyword_in_statement_middle = \
  else-in-statement-middle.test \
  if-in-statement-middle.test \
  while-in-statement-middle.test \
  for-in-statement-middle.test \
  do-in-statement-middle.test \
  repeat-in-statement-middle.test \
  until-in-statement-middle.test \
  switch-in-statement-middle.test \
  case-in-statement-middle.test \
  default-in-statement-middle.test \
  next-in-statement-middle.test \
  break-in-statement-middle.test
tests_keyword_in_statement_middle_f77 = \
  $(tests_keyword_in_statement_middle:.test=_f77.test)
generated_tests += \
  $(tests_keyword_in_statement_middle) \
  $(tests_keyword_in_statement_middle_f77)

EXTRA_DIST += keyword-in-statement-middle.sh
$(tests_keyword_in_statement_middle:.test=.log): \
  keyword-in-statement-middle.sh

EXTRA_DIST += keyword-in-statement-middle_f77.sh
$(tests_keyword_in_statement_middle_f77:.test=.log): \
  keyword-in-statement-middle_f77.sh

$(tests_keyword_in_statement_middle) \
$(tests_keyword_in_statement_middle_f77): Makefile
	$(autogen_sh__pre)
	$(AM_V_GEN)case '$@' in \
	   *_f77.test) a='_f77';; \
	   *.test) a='';; \
	   *) echo "Invalid script \`$@' to be generated" >&2; exit 1;; \
	 esac \
	   && helper="keyword-in-statement-middle$$a.sh" \
	   && stmt=`echo $@ | sed "s/-in-statement-middle$$a\.test$$//"` \
	   && echo "stmt='$$stmt'" >> $@-t \
	   && echo "rat4t_require_helper_script '$$helper'" >> $@-t
	$(autogen_sh__post)

#
# Ratfor control structures with empty bodies.
#

tests_control_structure_with_empty_body = \
  if-with-empty-body.test \
  if-else-with-empty-body.test \
  else-with-empty-body.test \
  while-with-empty-body.test \
  for-with-empty-body.test \
  do-with-empty-body.test \
  repeat-with-empty-body.test \
  repeat-until-with-empty-body.test \
  switch-with-empty-body.test \
  case-with-empty-body.test \
  default-with-empty-body.test
tests_control_structure_with_empty_body_f77 = \
  $(tests_control_structure_with_empty_body:.test=_f77.test)
generated_tests += \
  $(tests_control_structure_with_empty_body) \
  $(tests_control_structure_with_empty_body_f77)

EXTRA_DIST += control-structure-with-empty-body.sh
$(tests_control_structure_with_empty_body:.test=.log): \
  control-structure-with-empty-body.sh

EXTRA_DIST += control-structure-with-empty-body_f77.sh
$(tests_control_structure_with_empty_body_f77:.test=.log): \
  control-structure-with-empty-body_f77.sh

$(tests_control_structure_with_empty_body) \
$(tests_control_structure_with_empty_body_f77): Makefile
	$(autogen_sh__pre)
	$(AM_V_GEN)case '$@' in \
	   *_f77.test) a='_f77';; \
	   *.test) a='';; \
	   *) echo "Invalid script \`$@' to be generated" >&2; exit 1;; \
	 esac \
	   && helper="control-structure-with-empty-body$$a.sh" \
	   && stmt=`echo $@ | sed "s/-with-empty-body$$a\.test$$//"` \
	   && echo "stmt='$$stmt'" >> $@-t \
	   && echo "rat4t_require_helper_script '$$helper'" >> $@-t
	$(autogen_sh__post)


## Testsuite environment and setup.
include ../am/TestsEnvironment.am

## Testsuite cleanup.
include ../am/LocalCleanup.am

## Consistency checks on the list of tests.
include ../am/CheckListOfTests.am
maintainer-check-list-of-tests: $(generated_tests)

## Work around a bug of FreeBSD make w.r.t. parallel tests.
include ../am/WorkaroundParallelTests.am

## vim: ft=automake noet sw=4 ts=4
