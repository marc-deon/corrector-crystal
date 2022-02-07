---
Last updated: 22-11-14 at 23:33
---

# Actions

Actions are what players generally think of as states or moves of a character.
Idling, jumping, walking, crouching, attacks, flinching are all actions.
The most important things that Actions contain are hitboxes, damage info,
priority, links to Animations, and links to other Actions.

Note that attacks, especially \*multi-hit attacks, may (\*must) be made up of
multiple Actions; the hitboxes on an Action act as a composite hitbox,
are all activated and deactivated at the same time, and have the same damage.

Most parameters are optional and have default values, but not all do.

All paramters are named in lowerCamelCase.

# Parameters
Actions have the following paramters:
<!-- TOC -->
- [name](#name)
- [linksTo](#linksto)
- [mustLinkAfter](#mustLinkAfter)
- [state](#state)
- [priority](#priority)
- [animation](#animation)
- [damage](#damage)
- [hitstun](#hitstun)
- [hitStop](#hitStop)
- [forceAir](#forceair)
- [overrideGravity](#overridegravity)
- [knockback](#knockback)
- [airKnockback](#airknockback)
- [knockbackFrames](#knockbackframes)
- [selfKnockback](#selfknockback)
- [selfKnockbackFrames](#selfknockbackframes)
- [wallSplat](#wallsplat)
- [dunk](#dunk)
- [hardKnockdown](#hardknockdown)
- [hitboxes](#hitboxes)
- [hurtboxes](#hurtboxes)
- [shovebox](#shovebox)
- [step](#step)
- [stepDelay](#stepdelay)
- [overrideSelfVelocity](#overrideselfvelocity)
- [overrideSelfGravity](#overrideselfgravity)
- [phase](#phase)
<!-- /TOC -->

---

## name

Name of Action. Used to link Actions together and displayed in training mode.
- Example value: "Idle"
---

## linksTo
After [`mustLinkAfter`](#mustLinkAfter) frames, Action will automatically transition to
another Action which has a [`name`](#name) that is [`linksTo`](#linksTo).

Can be set to null to link to self.
- Example value: "Idle"
---

## mustLinkAfter
The number of frames (60 FPS) it takes for the Action to time out.

If `mustLinkAfter` is -1, then the action will never time out.
This is useful for idling, crouch, falling, etc.
- Example value: -1
---

## state
If `state` is 1, then the character will be put into an aerial state upon
entering the action.

Technically a bitflag, but 1 and 0 are the only values used as of writing this.
Consider replacing this with a better named, stricter parameter.
- Example value: 1
- Default value: 0
---

## priority
Actions with higher `priority` can interrupt those with lower `priority`.

Negative `priority` Actions can cancel into anything, and be canceled into from
any Action with an absolute `priority` lower than the negative Action, e.x.

Valid chain:
    j.C (80) ADC (-101) J.C (80) Super (120)

Invalid chain:
    J.C (80) Super (120) (!) ADC (-100) Super (120)

The second chain is invalid because |120| is greater than |-100|.

Also, as a special case for walking forward/backward, priority 1 can be canceled
into priority 1.
*This is neccesary to avoid moonwalking.* Make sure to use this for every
character.

- Example value: 0
---

## animation
[`Name`](Animation.md#name) of the animation to play upon starting this Action.
- Example value: "Idle"
- Default value: action name
---

## damage
How much unscaled damage opponent will take when they trigger any
of this Action's [`hitboxes`](#hitboxes). Unused if no [`hitboxes`](#hitboxes);
leave at default of 0.
- Example value: 1000
- Default value: 0
---

## hitstun
How many frames of hitstun opponent should suffer when [`hitboxes`](#hitboxes)
are triggered.
- Example value: 40
- Default value: 0
---

## hitStop
Before [`knockback`](#knockback) is applied, opponent is stationary in
a flinching freezeframe for this many frames.

- Example value: 4
- Default value: 0
---

## forceAir
Force opponent into air upon [`hitbox`](#hitboxes) connection.
For launchers, wallsplats, and dunks.

- Example value: true
- Default value: false
---

## overrideGravity
Override opponent's gravity with [`knockback`](#knockback) as new gravity until opponent
exits histun (wallsplats, bounces, hit again and times out).

To disable, leave at the default false.
- Example value: true
- Default value: false
---

## knockback
XY knockback per frame. On block can maybe be doubled or something.

When [`overrideGravity`](#overrideGravity) is true, this is new the value of gravity.
- Example value: [10,0]
- Default value: [0,0]
---

## airKnockback
Knockback against an airborn opponent. Generally leave at default,
unless applying a wallsplat or a dunk.

- Example value: [0,-10]
- Default value: [4,-4]
---

## knockbackFrames
How many frames [`knockback`](#knockback)/[`airKnockback`](#airKnockback) lasts.

Ignored on air counterhit; that state will last until landing.

- Example value: 5,
- Default value: 0
---

## selfKnockback
The pushback this character experiences when character hits opponent.

- Example value: [-10,0]
- Default value: [0,0]
---

## selfKnockbackFrames
How many frames [`selfKnockback`](#selfKnockback) is applied for.

- Example value: 5
- Default value: 0
---

## wallSplat
If true, then opponent will be locked out of tech state until
they hit a wall.

- Example value: true
- Default value: false
---

## dunk
If true, then opponent will be locked out of tech state until
they hit a floor.

- Example value: true
- Default value: false
---

## hardKnockdown
If true, then opponent will suffer hardKnockdown upon hitting the floor.

- Example value: true
- Default value: false
---

## hitboxes
A list of hitboxes to activate during this Action. Each hitbox can be
a 4, 5, or 6 tuple of integers in the form [x, y, w, h, on, off].
On and off are the frames that the box is activated and deactivated.
if not specified, on defaults to 0, off defaults to infinity.

- Example value: [[-10, -60, 50, 35, 0, 24]]
- Default value: [[]]

---

## hurtboxes
A list of hurtboxes to activate during this Action. Each hurtbox can be
a 4, 5, or 6 tuple of integers in the form [x, y, w, h, on, off].
On and off are the frames that the box is activated and deactivated.
if not specified, on defaults to 0, off defaults to infinity.

- Example value: [[-20, -100, 35, 100],[-5, -65, 50, 45]]
- Default value: [[]]
---

## shovebox

- Example value: [-15, -97, 25, 95]
- Default value: [0,0,0,0]

---
## step

After [stepDelay](#stepDelay]) frames, the character will be moved by this distance. Useful for move like Guile's back short (4MP) 3rd Strike Mokoto's forward roundhouse (6HK), or F-Roa's 6C.

- Example value: [60,0]
- Default value: [0,0]

---

## stepDelay

Delay before [step](#step) is applied, measured in 60FPS frames.

- Example value: 40
- Default value: 0

---

## overrideSelfVelocity

Set own velocity when entering this action. Useful for airdashes.
Only used if [overrideSelfTime](#overrideSelfTime) is not 0.

To maintain momentum Guilty Gear style, just go from AD -> fall.
For a more Melty Blood, start-stop feel, go from AD -> a short AD ender
with a smaller overrideSelfVelocity -> fall.

- Example value: [4,0]
- Default Value: [0,0]

---

## overrideSelfGravity

Temporarily set own gravity when entering this action. Useful for airdashes.
Only applicable in air, only applies for [overrideSelfGravity](#overrideSelfGravity) frames.

- Example value: [0,0]
- Default value: [0,0]

---

# overrideSelfTime

Number of frames that [overrideSelfGravity](#overrideSelfGravity) is active.
Also determines if [overrideSelfVelocity](#overrideSelfVelocity) is used.

- Example value: 8
- Default value: 0

# phase

For the duration of this action, the characters will not shove-collide (all hitboxes still interact).
Used for rolls, back-grabs, etc.

- Example value: true
- Default value: false