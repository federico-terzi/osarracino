import pygame, json, sys

pygame.init()
pygame.font.init()
fnt = pygame.font.SysFont("Arial", 30)

BLACK = ( 0, 0, 0)
GRAY = ( 96,96,96)
WHITE = ( 255, 255, 255)
BLUE = ( 92,142,255)
ORANGE = ( 255,178,102)
YELLOW= ( 243,232,102)
RED = ( 255, 0, 0)

CSIZE = 50


size = (510, 500)
screen = pygame.display.set_mode(size)
pygame.display.set_caption("Tablut Mode Editor")

board = [[0,1,1,2,2,2,1,1,0],
         [1,0,0,0,2,0,0,0,1],
         [1,0,0,0,0,0,0,0,1],
         [2,0,0,0,0,0,0,0,2],
         [2,2,0,0,3,0,0,2,2],
         [2,0,0,0,0,0,0,0,2],
         [1,0,0,0,0,0,0,0,1],
         [1,0,0,0,2,0,0,0,1],
         [0,1,1,2,2,2,1,1,0]]

matrix =[[0,0,0,0,0,0,0,0,0],
         [0,0,0,0,0,0,0,0,0],
         [0,0,0,0,0,0,0,0,0],
         [0,0,0,0,0,0,0,0,0],
         [0,0,0,0,0,0,0,0,0],
         [0,0,0,0,0,0,0,0,0],
         [0,0,0,0,0,0,0,0,0],
         [0,0,0,0,0,0,0,0,0],
         [0,0,0,0,0,0,0,0,0]]

def load_matrix(json, matrix):
    for y, row in enumerate(json["board"]):
        for x, cell in enumerate(row):
            if cell == "WHITE":
                matrix[y][x] = 2
            elif cell == "BLACK":
                matrix[y][x] = 1
            elif cell == "KING":
                matrix[y][x] = 3

if len(sys.argv) > 1:
    option = sys.argv[1]
    if option == "default":
        load_matrix(json.loads('{"board": [["EMPTY", "EMPTY", "EMPTY", "BLACK", "BLACK", "BLACK", "EMPTY", "EMPTY", "EMPTY"], ["EMPTY", "EMPTY", "EMPTY", "EMPTY", "BLACK", "EMPTY", "EMPTY", "EMPTY", "EMPTY"], ["EMPTY", "EMPTY", "EMPTY", "EMPTY", "WHITE", "EMPTY", "EMPTY", "EMPTY", "EMPTY"], ["BLACK", "EMPTY", "EMPTY", "EMPTY", "WHITE", "EMPTY", "EMPTY", "EMPTY", "BLACK"], ["BLACK", "BLACK", "WHITE", "WHITE", "KING", "WHITE", "WHITE", "BLACK", "BLACK"], ["BLACK", "EMPTY", "EMPTY", "EMPTY", "WHITE", "EMPTY", "EMPTY", "EMPTY", "BLACK"], ["EMPTY", "EMPTY", "EMPTY", "EMPTY", "WHITE", "EMPTY", "EMPTY", "EMPTY", "EMPTY"], ["EMPTY", "EMPTY", "EMPTY", "EMPTY", "BLACK", "EMPTY", "EMPTY", "EMPTY", "EMPTY"], ["EMPTY", "EMPTY", "EMPTY", "BLACK", "BLACK", "BLACK", "EMPTY", "EMPTY", "EMPTY"]], "turn": "WHITE"}'), matrix)
    else:
        load_matrix(json.loads(option), matrix)


carryOn = True 
clock = pygame.time.Clock()
 
# -------- Main Program Loop -----------
while carryOn:
    # --- Main event loop
    for event in pygame.event.get(): # User did something
        if event.type == pygame.MOUSEBUTTONUP:
            pos = pygame.mouse.get_pos()
            x = int(pos[0]/CSIZE)
            y = int(pos[1]/CSIZE)
            if x>8 or y>8:
                continue
            matrix[y][x] += 1
            if matrix[y][x] > 3:
                matrix[y][x] = 0
                
            # Export the  matrix layout
            output = []
            for y, row in enumerate(matrix):
                output.append([])
                for x, cell in enumerate(row):
                    if cell == 1:
                        output[y].append("BLACK")
                    elif cell == 2:
                        output[y].append("WHITE")
                    elif cell == 3:
                        output[y].append("KING")
                    elif board[y][x] == 3:
                        output[y].append("THRONE")
                    else:
                        output[y].append("EMPTY")
            print()
            print(json.dumps({"board":output, "turn": "WHITE"}))            

        if event.type == pygame.QUIT: # If user clicked close
            carryOn = False # Flag that we are done so we exit this loop
 
    screen.fill(BLACK)
    
    # Draw the board
    for y, row in enumerate(board):
        for x, cell in enumerate(row):
            color = YELLOW
            if cell == 2:
                color = GRAY
            elif cell == 1:
                color = BLUE
            elif cell == 3:
                color = ORANGE
            pygame.draw.rect(screen, color, [x*CSIZE+2, y*CSIZE+2, CSIZE-2, CSIZE-2])
            
            # Render the pawn
            if matrix[y][x] > 0:
                colors = [BLACK, WHITE, RED]
                pygame.draw.circle(screen, colors[matrix[y][x]-1], (x*CSIZE+int(CSIZE/2), y* CSIZE + int(CSIZE/2)), 20)
        text = fnt.render(str(y+1), True, WHITE)
        screen.blit(text, (9*CSIZE + 10, y*CSIZE+10))

    for col in range(9):
        text = fnt.render(str(chr(ord('a')+col)).upper(), True, WHITE)
        screen.blit(text, (col * CSIZE + 15, 9*CSIZE+5))

    # --- Go ahead and update the screen with what we've drawn.
    pygame.display.flip()

    # --- Limit to 60 frames per second
    clock.tick(60)

#Once we have exited the main program loop we can stop the game engine:
pygame.quit()
