
add_subdirectory(GUI)
add_subdirectory(instructions)

set(STARVAX_SOURCES
  ${STARVAX_SOURCES}
   src/sequencer/Sequencer.h
   src/sequencer/Sequencer_Functionality.cpp
   src/sequencer/Sequencer_Functionality.h
   src/sequencer/Sequencer_Instruction.cpp
   src/sequencer/Sequencer_Instruction.h
   src/sequencer/Sequencer_InstructionFactory.cpp
   src/sequencer/Sequencer_InstructionFactory.h
   src/sequencer/Sequencer_InstructionFactory_IF.h
   src/sequencer/Sequencer_LineParser.cpp
   src/sequencer/Sequencer_LineParser.h
   src/sequencer/Sequencer_Runner.cpp
   src/sequencer/Sequencer_Runner.h
   src/sequencer/Sequencer_ScriptParser.cpp
   src/sequencer/Sequencer_ScriptParser.h
   src/sequencer/Sequencer_ScriptSequencer.cpp
   src/sequencer/Sequencer_ScriptSequencer.h
   src/sequencer/SequencerFactory.cpp
   src/sequencer/SequencerFactory.h
  PARENT_SCOPE
)

set ( ALL_SRC_DIRS
   ${ALL_SRC_DIRS}
   ${CMAKE_CURRENT_SOURCE_DIR}
   PARENT_SCOPE
)

