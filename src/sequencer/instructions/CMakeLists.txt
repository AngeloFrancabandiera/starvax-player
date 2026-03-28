
set(STARVAX_SOURCES
  ${STARVAX_SOURCES}
   src/sequencer/instructions/Sequencer_EndInstruction.cpp
   src/sequencer/instructions/Sequencer_EndInstruction.h
   src/sequencer/instructions/Sequencer_EntryInstruction.cpp
   src/sequencer/instructions/Sequencer_EntryInstruction.h
   src/sequencer/instructions/Sequencer_HallInstruction.cpp
   src/sequencer/instructions/Sequencer_HallInstruction.h
   src/sequencer/instructions/Sequencer_LightInstruction.cpp
   src/sequencer/instructions/Sequencer_LightInstruction.h
   src/sequencer/instructions/Sequencer_PlaylistInstruction.cpp
   src/sequencer/instructions/Sequencer_PlaylistInstruction.h
   src/sequencer/instructions/Sequencer_RepeatInstruction.cpp
   src/sequencer/instructions/Sequencer_RepeatInstruction.h
   src/sequencer/instructions/Sequencer_SequenceInstruction.cpp
   src/sequencer/instructions/Sequencer_SequenceInstruction.h
  PARENT_SCOPE
)

set ( ALL_SRC_DIRS
   ${ALL_SRC_DIRS}
   ${CMAKE_CURRENT_SOURCE_DIR}
   PARENT_SCOPE
)