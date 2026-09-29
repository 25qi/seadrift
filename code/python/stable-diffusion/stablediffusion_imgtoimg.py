# -*- coding: utf-8 -*-
import getpass, os
from datetime import datetime
from dotenv import load_dotenv

load_dotenv()

# Our host url should not be prepended with "https" nor should it have a trailing slash.
os.environ['STABILITY_HOST'] = 'grpc.stability.ai:443'

# Sign up for an account at the following link to get an API Key. https://beta.dreamstudio.ai/membership
# Click on the following link once you have created an account to be taken to your API Key. Paste it below when prompted after running the cell. https://beta.dreamstudio.ai/membership?tab=apiKeys
# STABILITY_KEY is read from the environment (see .env.example)

#@title <a name="Step 3"><font color="#FFFFFF">3. Import additional dependencies and establish our connection to the API.</font></a>

import io
import os
import warnings

from IPython.display import display
from PIL import Image
from stability_sdk import client
import stability_sdk.interfaces.gooseai.generation.generation_pb2 as generation


# Set up our connection to the API.
stability_api = client.StabilityInference(
    key=os.environ['STABILITY_KEY'], # API Key reference.
    verbose=True, # Print debug messages.
    engine="stable-diffusion-v1-5", # Set the engine to use for generation. For SD 2.0 use "stable-diffusion-v2-0".
    # Available engines: stable-diffusion-v1 stable-diffusion-v1-5 stable-diffusion-512-v2-0 stable-diffusion-768-v2-0 stable-inpainting-v1-0 stable-inpainting-512-v2-0
)

#@title <a name="Step 4"><font color="#FFFFFF">4. Set up initial generation parameters, display image on generation, and safety warning for if the adult content classifier is tripped.</font></a>

"""# Set up our initial generation parameters.
answers = stability_api.generate(
    prompt="ernst haeckel style. blue flower. orange fruit. plant has many leaves, each leave is about 0.5cm wide and 2cm long. Large leaves have dark and light colors, with a strong herbaceous stem",
    # seed=892226758, # If a seed is provided, the resulting generated image will be deterministic.
                    # What this means is that as long as all generation parameters remain the same, you can always recall the same image simply by generating it again.
                    # Note: This isn't quite the case for Clip Guided generations, which we'll tackle in a future example notebook.
    steps=30, # Amount of inference steps performed on image generation. Defaults to 30. 
    cfg_scale=5.0, # Influences how strongly your generation is guided to match your prompt.
                   # Setting this value higher increases the strength in which it tries to match your prompt.
                   # Defaults to 7.0 if not specified.
    width=512, # Generation width, defaults to 512 if not included.
    height=512, # Generation height, defaults to 512 if not included.
    sampler=generation.SAMPLER_K_DPMPP_2M # Choose which sampler we want to denoise our generation with.
                                                 # Defaults to k_dpmpp_2m if not specified. Clip Guidance only supports ancestral samplers.
                                                 # (Available Samplers: ddim, plms, k_euler, k_euler_ancestral, k_heun, k_dpm_2, k_dpm_2_ancestral, k_dpmpp_2s_ancestral, k_lms, k_dpmpp_2m)
)

# Set up our warning to print to the console if the adult content classifier is tripped.
# If adult content classifier is not tripped, display generated image.
for resp in answers:
    for artifact in resp.artifacts:
        if artifact.finish_reason == generation.FILTER:
            warnings.warn(
                "Your request activated the API's safety filters and could not be processed."
                "Please modify the prompt and try again.")
        if artifact.type == generation.ARTIFACT_IMAGE:
            img = Image.open(r"/Users/hongyinull/Desktop/IPTA畢製海漂計畫/海漂植物生成/220130test.png")
            display(img)"""

#@title <a name="Step 5"><font color="#FFFFFF">5. Set up an initial image based on our previous generation, and use a prompt to convert it into a crayon drawing.</font></a>

img = Image.open(r"/Users/hongyinull/Desktop/IPTA畢製海漂計畫/海漂植物生成/220130test.png")
# 這邊的參數我精挑細選過啦，不過也可以換換看會不會效果更好 Set up our initial generation parameters.
answers = stability_api.generate(
    prompt="ernst haeckel style. realistic plant grown from a transparent glass ball. big giant magical flower with magic on it, cute sweet fruit. with a girl stem and witch leaves.ernst haeckel",
    init_image=img, # Assign our previously generated img as our Initial Image for transformation.
    start_schedule=0.75, # Set the strength of our prompt in relation to our initial image.
    # seed=2778030292, #勿動這個精巧精挑萬選的種子碼！ # If attempting to transform an image that was previously generated with our API,
                    # initial images benefit from having their own distinct seed rather than using the seed of the original image generation.
    steps=30, # Amount of inference steps performed on image generation. Defaults to 30. 
    cfg_scale=10.0, # Influences how strongly your generation is guided to match your prompt.
                   # Setting this value higher increases the strength in which it tries to match your prompt.
                   # Defaults to 7.0 if not specified.
    width=512, # Generation width, defaults to 512 if not included.
    height=256, # Generation height, defaults to 512 if not included.
    sampler=generation.SAMPLER_K_EULER # Choose which sampler we want to denoise our generation with.
                                                 # Defaults to k_dpmpp_2m if not specified. Clip Guidance only supports ancestral samplers.
                                                 # (Available Samplers: ddim, plms, k_euler, k_euler_ancestral, k_heun, k_dpm_2, k_dpm_2_ancestral, k_dpmpp_2s_ancestral, k_lms, k_dpmpp_2m)
)

# Set up our warning to print to the console if the adult content classifier is tripped.
# If adult content classifier is not tripped, display generated image.
for resp in answers:
    for artifact in resp.artifacts:
        if artifact.finish_reason == generation.FILTER:
            warnings.warn(
                "Your request activated the API's safety filters and could not be processed."
                "Please modify the prompt and try again.")
        if artifact.type == generation.ARTIFACT_IMAGE:
            img2 = Image.open(io.BytesIO(artifact.binary)) # Set our resulting initial image generation as 'img2' to avoid overwriting our previous 'img' generation.
            today = datetime.now()
            img2.save(str(today.strftime("%y%m%d%H%M%S"))+ "StableDiffusion.png")
            # img2.save(str(artifact.seed)+ "StableDiffusion.png")