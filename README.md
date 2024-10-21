# Entityform

The Entityform module enables you to create front-end forms (fieldable
entities), which contain fields that can be defined. These forms use the
standard Backdrop fields. This means that out of the box, the user can use any
standard Backdrop node field.

## Dependencies

- Entity UI
- Entity Plus

No dependency, but recommended, especially if you're using this module in
combination with Rules: [Entity Tokens](https://backdropcms.org/project/entity_token)

## Installation

- Install this module using the
  [official Backdrop CMS instructions](https://docs.backdropcms.org/documentation/extend-with-modules)

## Configuration

1. Go to Structure > Entityform Types (admin/structure/entityform_types) and "Add an Entityform Type".
2. Select manage fields tab and add the desired fields
3. Select manage displays tab and adapt as needed
4. You can now preview the form and permitted roles can submit values

## Issues

Bugs and feature requests should be reported in the
[issue queue](https://github.com/backdrop-contrib/entityform/issues).

## Known Issues

Submodules only *partly* work.

- entityform_anonymous seems fully functional
- entityform_i18n partly, see [this issue](https://github.com/backdrop-contrib/entityform/issues/21)
- entityform_notifications can not work, see [this issue](https://github.com/backdrop-contrib/entityform/issues/23)

## Maintainers

- [indigoxela](https://github.com/indigoxela)
- Seeking additional maintainers

## Credits

Ported to Backdrop by [Andy Shillingford](https://github.com/docwilmot/)

Original Drupal development by

* Joël Pittet (joelpittet) - https://www.drupal.org/u/joelpittet
* Ted Bowman (tedbow) - https://www.drupal.org/u/tedbow

Supporting Organizations for Drupal version:

* Six Mile Tech - https://www.drupal.org/six-mile-tech
* The University of British Columbia -
  https://www.drupal.org/the-university-of-british-columbia

## License

This project is GPL v2 software.
See the LICENSE.txt file in this directory for complete text.
