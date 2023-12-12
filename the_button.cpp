//
//
//

#include "the_button.h"


void TheButton::init(TheButtonInfo* i) {
    setIcon( *(i->icon) );
    info =  i;

    // Connect the toggled signal to the clicked slot
    connect(this, &TheButton::toggled, this, &TheButton::clicked);
}


void TheButton::clicked() {
    emit jumpTo(info);
    emit likeStateChanged(isChecked());
}
