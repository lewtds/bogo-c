## Naming Convention

There are two classes of special markings used in Vietnamese.
We use the word 'tone' to refer to marks on vowels that change
their pitch. There are 6 such tones in Vietnamese:

* hanging, marked by a grave (à)
* sharp, marked by an acute mark (á)
* asking, marked by a hook (ả)
* tumbling, marked by a tilde (ã)
* heavy, marked by an under dot (ạ), and finally
* level or no tone

The other kind of marks are ones that create new letters from
existing ones. We call these simply 'marks'.

* circumflex, or hat (^) used in â, ê, ô
* breve, used in ă
* horn, used in ư, ơ
* dash, used in đ

## Coding Style & Decisions

- Use **camelCase** for functions and variables/arguments, **PascalCase** for
  structs, **ALL_CAPS** for enums.
- Kernighan & Ritchie style formatting/indenting. Expand tab to 4 spaces.
- Keep `typedef`s to the minimum.
- Each struct should have a constructor, prefixed with *new*, e.g. `newMyStruct`.
- Use BSD's `queue.h` for list operations. See [this tutorial][1] (Vietnamese).

[1]: http://ngochin.com/2014/01/01/linked-list-preprocessor-c/

## Operation

Bogo engine works in two phases. First, it tries to build a list of
transformations holding information on each character in the raw input string.

For example, if the user press buttons on the keyboard in the sequence "meof"
then the list should look similar to this:

    [
        { 'type' : 'append',
          'key': 'm' },
        { 'type' : 'append',
          'key': 'e' },
        { 'type' : 'append',
          'key': 'o' },
        { 'type' : 'tone',
          'key': 'f',
          'tone': 'GRAVE',
          'target': 1 }
    ]

Using matching rules, it will sort each key into three categories: tone, mark,
append; and find one or more suitable targets for it.

The second phase is simpler, Bogo flattens the list into a final string using
the instructions built from the first phase into the list itself.

This two-phase structure makes certain operations like undoing, handling
backspace or returning raw string after some backspaces relatively easy. In the
example above, if we want to undo the f-TONE-GRAVE operation (mèo -> meo) then
we can simply remove the last TONE or MARK member of the list and reflatten it.

Handling backspaces is a bit more complicated. The users expect backspace to
delete characters, not undo tone or mark operations. So we remove the last
appending transformation and all other operations that target it. If the
targeting transformation has more than one targets then remove the orphaned
target from it.

**WARNING:** The following text is deprecated as `target` is now a pointer
to a single transformation in the list. Shortcuts like *uow* are now handled
by regular expressions and/or manual patching.

Consider this example:

    [
        { 'type': 'APPEND',
          'key': 'h'},
        { 'type': 'APPEND',
          'key': 'u' },
        { 'type': 'APPEND',
          'key': 'o' },
        { 'type': 'MARK',
          'key': 'w',
          'mark': 'HORN',
          'target': [1, 2] }
    ]

which should be "hươ" when flattened. The users expect backspace would delete
"ơ" and leave "ư" intact. To do that, we remove the last appending
transformation, which is o-APPEND and remove the "2" target from w-MARK-HORN.
That gives "hư", as expected.

Now what happens if the user presses "k" after all that? "hưk" is not
Vietnamese, so we return a plain ASCII string. Now the ASCII string could be
"huwk" or "huowk". The former is probably less astonishing (google POLA) since
the user explicitly deleted "o" and would not expect to see it again.

So after receiving "k", we modify the list into this:

    [
        { 'type': 'APPEND',
          'key': 'h'},
        { 'type': 'APPEND',
          'key': 'u' },
        { 'type': 'APPEND',
          'key': 'w' }
        { 'type': 'APPEND',
          'key': 'k' }
    ]

And flatten it into "huwk".
